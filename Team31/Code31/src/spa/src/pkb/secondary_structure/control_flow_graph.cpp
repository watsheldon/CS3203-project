#include "control_flow_graph.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <numeric>
#include <set>

#include "common/aliases.h"
#include "common/entity_type_enum.h"

namespace spa {
ControlFlowGraph::ControlFlowGraph(std::size_t stmt_count,
                                   std::size_t stmtlst_count,
                                   Stores stores) noexcept
        : stmt_node_index_(stmt_count + 1, 0),
          stmtlst_stmt_(stores.stmtlst_statements),
          type_store_(stores.type_statements),
          stmtlst_parent_(stores.stmtlst_parent) {
    nodes_.reserve(stmtlst_count);
    nodes_.emplace_back(0);  // dummy node
    prev_.reserve(stmt_count);
    next_.reserve(stmt_count);
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
    auto& while_node = GetNode(while_stmt);
    if (prev_stmt != 0) while_node.prev.emplace_back(prev_stmt);
    StmtLstIndex while_stmtlst_idx =
            stmtlst_parent_.GetWhileStmtLst(while_stmt);
    const auto& while_stmtlst = stmtlst_stmt_.GetStatements(while_stmtlst_idx);
    StmtNo last_stmt = while_stmtlst.back();
    if (type_store_.GetType(last_stmt) == StmtType::kIf) {
        ProcessWhileExitPoints(while_node, last_stmt, while_stmt);
    } else {
        while_node.prev.emplace_back(last_stmt);
        auto& node = GetNode(last_stmt);
        (node.next.first ? node.next.second : node.next.first) = while_stmt;
    }
    StmtNo first_stmt = while_stmtlst.front();
    StmtNo after_stmt = stmtlst_stmt_.GetFollowsSecondArg(while_stmt);
    while_node.next = {first_stmt, after_stmt};
    auto &first_node = GetNode(first_stmt), &after_node = GetNode(after_stmt);
    first_node.prev.emplace_back(while_stmt);
    after_node.prev.emplace_back(while_stmt);
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
        stmt_node_index_[last] = stmt_node_index_[first_stmt];
    }
    return last;
}
void ControlFlowGraph::Precompute() noexcept {
    for (StmtNo i = 1; i < stmt_node_index_.size(); ++i) {
        const auto& node = GetNode(i);
        if (i != node.stop || node.next.first != 0) prev_.emplace_back(i);
        if (i != node.start || !node.prev.empty()) next_.emplace_back(i);
    }
}
bool ControlFlowGraph::HasNext() const noexcept {
    static const bool kResult =
            std::any_of(nodes_.begin(), nodes_.end(), [](const auto& n) {
                return n.start != n.stop || n.next.first != 0;
            });
    return kResult;
}
PairOf<StmtNo> ControlFlowGraph::GetNextPair(StmtNo prev) const noexcept {
    if (prev >= stmt_node_index_.size()) return {0, 0};
    const auto& curr_node = GetNode(prev);
    if (prev == curr_node.stop) {
        return curr_node.next;
    }
    assert(curr_node.start < curr_node.stop);
    return {prev + 1, 0};
}
bool ControlFlowGraph::HasNext(StmtNo prev) const noexcept {
    return GetNextPair(prev).first != 0;
}
bool ControlFlowGraph::HasPrev(StmtNo next) const noexcept {
    if (next >= stmt_node_index_.size()) return false;
    const auto& node = GetNode(next);
    return next != node.start || !node.prev.empty();
}
bool ControlFlowGraph::IsNext(StmtNo prev, StmtNo next) const noexcept {
    if (prev >= stmt_node_index_.size() || next >= stmt_node_index_.size())
        return false;
    const auto& prev_node = GetNode(prev);
    return prev_node.next.first == next || prev_node.next.second == next;
}
ControlFlowGraph::Node& ControlFlowGraph::GetNode(StmtNo stmt_no) noexcept {
    assert(stmt_no < stmt_node_index_.size());
    NodeIndex& index = stmt_node_index_[stmt_no];
    if (index == 0) {
        index = (int)nodes_.size();
        return nodes_.emplace_back(stmt_no);
    }
    return nodes_[index];
}
const ControlFlowGraph::Node& ControlFlowGraph::GetNode(
        StmtNo stmt_no) const noexcept {
    assert(stmt_no < stmt_node_index_.size());
    NodeIndex index = stmt_node_index_[stmt_no];
    assert(index != 0);
    return nodes_[index];
}
std::set<StmtNo> ControlFlowGraph::GetPrev(StmtNo next,
                                           StmtType stmt_type) const noexcept {
    if (next >= stmt_node_index_.size()) return {};
    const auto& node = GetNode(next);
    return next == node.start ? FilterVecResult(node.prev, stmt_type)
                              : std::set<StmtNo>{next - 1};
}
std::set<StmtNo> ControlFlowGraph::GetNext(StmtNo prev,
                                           StmtType stmt_type) const noexcept {
    if (prev >= stmt_node_index_.size()) return {};
    return FilterPairResult(GetNextPair(prev), stmt_type);
}
std::set<StmtNo> ControlFlowGraph::FilterPairResult(
        const PairOf<StmtNo>& pair, StmtType stmt_type) const noexcept {
    auto [first, second] = pair;
    std::set<StmtNo> ret;
    AddIfMatches(stmt_type, first, ret);
    AddIfMatches(stmt_type, second, ret);
    return ret;
}
void ControlFlowGraph::AddIfMatches(const StmtType stmt_type,
                                    const StmtNo stmt_no,
                                    std::set<StmtNo>& results) const {
    if (stmt_no != 0 && (stmt_type == StmtType::kAll ||
                         type_store_.GetType(stmt_no) == stmt_type))
        results.emplace(stmt_no);
}
std::set<StmtNo> ControlFlowGraph::FilterVecResult(
        const std::vector<StmtNo>& stmts, StmtType type) const {
    if (type == StmtType::kAll) return {stmts.begin(), stmts.end()};
    std::set<StmtNo> results;
    for (StmtNo s : stmts) {
        if (type_store_.GetType(s) == type) {
            results.emplace_hint(results.end(), s);
        }
    }
    return results;
}
std::set<StmtNo> ControlFlowGraph::GetNextSingleArg(
        ArgPos return_pos, StmtType stmt_type) const noexcept {
    const auto& column = return_pos == ArgPos::kFirst ? prev_ : next_;
    if (stmt_type == StmtType::kAll) return {column.begin(), column.end()};
    std::set<StmtNo> results;
    for (StmtNo s : column) {
        if (type_store_.GetType(s) == stmt_type) {
            results.emplace_hint(results.end(), s);
        }
    }
    return results;
}
PairVec<StmtNo> ControlFlowGraph::GetNextPairs(
        StmtType prev_type, StmtType next_type) const noexcept {
    if (prev_type == StmtType::kAll) {
        return next_type == StmtType::kAll
                       ? GetNextPairsNoFilter()
                       : GetNextPairsFilterByNext(next_type);
    }
    if (next_type == StmtType::kAll) return GetNextPairsFilterByPrev(prev_type);
    return GetNextPairsFilterBoth(prev_type, next_type);
}
PairVec<StmtNo> ControlFlowGraph::GetNextPairsNoFilter() const noexcept {
    PairVec<StmtNo> results;
    auto& [prev, next] = results;
    for (const auto& node : nodes_) {
        if (node.start == 0) continue;
        if (node.start != node.stop) {
            const int num_pairs = node.stop - node.start;
            prev.resize(prev.size() + num_pairs);
            next.resize(prev.size());
            std::iota(prev.end() - num_pairs, prev.end(), node.start);
            std::iota(next.end() - num_pairs, next.end(), node.start + 1);
        }
        if (node.next.first != 0) next.emplace_back(node.next.first);
        if (node.next.second != 0) next.emplace_back(node.next.second);
        prev.resize(next.size(), node.stop);
    }
    return results;
}
PairVec<StmtNo> ControlFlowGraph::GetNextPairsFilterByNext(
        StmtType type) const noexcept {
    PairVec<StmtNo> results;
    auto& [prev, next] = results;
    for (StmtNo n : type_store_.GetStatements(type)) {
        const auto& node = GetNode(n);
        if (n != node.start) {
            prev.emplace_back(n - 1);
            next.emplace_back(n);
            continue;
        }
        prev.insert(prev.end(), node.prev.begin(), node.prev.end());
        next.resize(prev.size(), n);
    }
    return results;
}
PairVec<StmtNo> ControlFlowGraph::GetNextPairsFilterByPrev(
        StmtType type) const {
    PairVec<StmtNo> results;
    auto& [prev, next] = results;
    for (StmtNo p : type_store_.GetStatements(type)) {
        const auto& node = GetNode(p);
        if (p != node.stop) {
            prev.emplace_back(p);
            next.emplace_back(p + 1);
            continue;
        }
        if (node.next.first != 0) next.emplace_back(node.next.first);
        if (node.next.second != 0) next.emplace_back(node.next.second);
        prev.resize(next.size(), p);
    }
    return results;
}
PairVec<StmtNo> ControlFlowGraph::GetNextPairsFilterBoth(
        StmtType prev_type, StmtType next_type) const {
    PairVec<StmtNo> results;
    auto& [prev, next] = results;
    for (StmtNo p : type_store_.GetStatements(prev_type)) {
        const auto& node = GetNode(p);
        if (p != node.stop) {
            if (type_store_.GetType(p + 1) == next_type)
                prev.emplace_back(p), next.emplace_back(p + 1);
            continue;
        }
        auto [first, second] = node.next;
        if (first != 0 && type_store_.GetType(first) == next_type)
            next.emplace_back(first);
        if (second != 0 && type_store_.GetType(second) == next_type)
            next.emplace_back(second);
        prev.resize(next.size(), p);
    }
    return results;
}
}  // namespace spa
