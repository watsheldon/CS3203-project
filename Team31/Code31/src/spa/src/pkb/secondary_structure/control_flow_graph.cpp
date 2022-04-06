#include "control_flow_graph.h"

#include <cassert>
#include <cstddef>
#include <queue>
#include <set>

#include "common/aliases.h"
#include "common/entity_type_enum.h"

namespace spa {
ControlFlowGraph::ControlFlowGraph(std::size_t stmt_count,
                                   std::size_t stmtlst_count,
                                   Stores stores) noexcept
        : stmt_node_index(stmt_count + 1, 0),
          stmtlst_stmt_(stores.stmtlst_statements),
          type_store_(stores.type_statements),
          stmtlst_parent_(stores.stmtlst_parent) {
    nodes.reserve(stmtlst_count * 3 / 2);
    nodes.emplace_back(0);  // dummy node
    prev.reserve(stmt_count);
    next.reserve(stmt_count);
    StmtNo curr = 0;
    while (++curr <= stmt_count) {
        auto curr_type = type_store_.GetType(curr);
        if (curr_type == StmtType::kIf) {
            AddIfNode(curr);
            continue;
        } else if (curr_type == StmtType::kWhile) {
            AddWhileNode(curr);
            continue;
        }
        curr = AddCombinedNode(curr);
    }
    Precompute();
}
void ControlFlowGraph::AddIfNode(StmtNo if_stmt, StmtNo prev_stmt) noexcept {
    auto& if_node = GetNode(if_stmt);
    if (prev_stmt != 0) if_node.prev.emplace_back(prev_stmt);
    auto [t, e] = stmtlst_parent_.GetIfStmtLst(if_stmt);
    auto then_next = stmtlst_stmt_.GetStatements(t).front();
    auto else_next = stmtlst_stmt_.GetStatements(e).front();
    if_node.next = {then_next, else_next};
    auto &then_node = GetNode(then_next), &else_node = GetNode(else_next);
    then_node.prev.emplace_back(if_stmt), else_node.prev.emplace_back(if_stmt);
    StmtNo follower = stmtlst_stmt_.GetFollowsSecondArg(if_stmt);
    if (follower == 0) return;
    std::vector<StmtLstIndex> stack = {e, t};
    while (!stack.empty()) {
        auto stmtlst_idx = stack.back();
        stack.pop_back();
        StmtNo curr_last_stmt = stmtlst_stmt_.GetStatements(stmtlst_idx).back();
        if (type_store_.GetType(curr_last_stmt) == StmtType::kIf) {
            auto [t1, e1] = stmtlst_parent_.GetIfStmtLst(curr_last_stmt);
            stack.emplace_back(e1), stack.emplace_back(t1);
        } else {
            auto& node = GetNode(curr_last_stmt);
            (node.next.first ? node.next.second : node.next.first) = follower;
            auto& follower_node = GetNode(follower);
            follower_node.prev.emplace_back(curr_last_stmt);
        }
    }
}
void ControlFlowGraph::AddWhileNode(StmtNo while_stmt,
                                    StmtNo prev_stmt) noexcept {
    if (prev_stmt != 0) GetNode(while_stmt).prev.emplace_back(prev_stmt);
    StmtLstIndex while_stmtlst_idx =
            stmtlst_parent_.GetWhileStmtLst(while_stmt);
    const auto& while_stmtlst = stmtlst_stmt_.GetStatements(while_stmtlst_idx);
    StmtNo last_stmt = while_stmtlst.back();
    if (type_store_.GetType(last_stmt) == StmtType::kIf) {
        ProcessWhileExitPoints(GetNode(while_stmt), last_stmt, while_stmt);
    } else {
        GetNode(while_stmt).prev.emplace_back(last_stmt);
        auto& after_node = GetNode(last_stmt);
        (after_node.next.first ? after_node.next.second
                               : after_node.next.first) = while_stmt;
    }
    StmtNo first_stmt = while_stmtlst.front();
    StmtNo after_stmt = stmtlst_stmt_.GetFollowsSecondArg(while_stmt);
    GetNode(while_stmt).next = {first_stmt, after_stmt};
    GetNode(first_stmt).prev.emplace_back(while_stmt);
    GetNode(after_stmt).prev.emplace_back(while_stmt);
}
void ControlFlowGraph::ProcessWhileExitPoints(Node& while_node,
                                              StmtNo last_stmt,
                                              StmtNo while_stmt) {
    auto [t, e] = stmtlst_parent_.GetIfStmtLst(last_stmt);
    std::vector<StmtLstIndex> stack = {e, t};
    while (!stack.empty()) {
        auto stmtlst_idx = stack.back();
        stack.pop_back();
        StmtNo curr_last_stmt = stmtlst_stmt_.GetStatements(stmtlst_idx).back();
        if (type_store_.GetType(curr_last_stmt) != StmtType::kIf) {
            while_node.prev.emplace_back(curr_last_stmt);
            auto& node = GetNode(curr_last_stmt);
            (node.next.first ? node.next.second : node.next.first) = while_stmt;
            continue;
        }
        auto [t1, e1] = stmtlst_parent_.GetIfStmtLst(curr_last_stmt);
        stack.emplace_back(e1), stack.emplace_back(t1);
    }
}
StmtNo ControlFlowGraph::AddCombinedNode(StmtNo first_stmt) noexcept {
    auto& curr_node = GetNode(first_stmt);
    StmtLstIndex stmtlst_idx = stmtlst_stmt_.GetStmtlst(first_stmt);
    const auto& stmtlst = stmtlst_stmt_.GetStatements(stmtlst_idx);
    StmtNo last = first_stmt;
    for (int i = stmtlst_stmt_.GetStmtRelativePos(first_stmt) + 1;
         i < stmtlst.size(); ++i) {
        last = stmtlst[i];
        auto stmt_type = type_store_.GetType(last);
        if (stmt_type == StmtType::kIf || stmt_type == StmtType::kWhile) {
            curr_node.next.first = last;
            curr_node.stop = stmtlst[i - 1];
            stmt_type == StmtType::kIf ? AddIfNode(last, stmtlst[i - 1])
                                       : AddWhileNode(last, stmtlst[i - 1]);
            break;
        }
        stmt_node_index[last] = stmt_node_index[first_stmt];
    }
    return last;
}
void ControlFlowGraph::Precompute() noexcept {
    for (auto n = ++nodes.begin(); n != nodes.end(); ++n) {
        if (!n->prev.empty()) {
            next.emplace_back(n->start);
        } else {
            roots.emplace_back(n->start);
        }
        if (n->next.first != 0) {
            prev.emplace_back(n->stop);
        }
        for (int i = n->start + 1; i <= n->stop; ++i) {
            prev.emplace_back(i - 1), next.emplace_back(i);
        }
    }
}
ControlFlowGraph::Node& ControlFlowGraph::GetNode(StmtNo stmt_no) noexcept {
    assert(stmt_no < stmt_node_index.size());
    NodeIndex& index = stmt_node_index[stmt_no];
    if (index == 0) {
        index = (int)nodes.size();
        return nodes.emplace_back(stmt_no);
    }
    return nodes[index];
}
const ControlFlowGraph::Node& ControlFlowGraph::GetNode(
        StmtNo stmt_no) const noexcept {
    assert(stmt_no < stmt_node_index.size());
    NodeIndex index = stmt_node_index[stmt_no];
    assert(index != 0);
    return nodes[index];
}
}  // namespace spa
