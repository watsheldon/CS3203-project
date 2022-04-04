#include "control_flow_graph.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <numeric>
#include <queue>
#include <set>
#include <utility>

#include "common/aliases.h"
#include "common/entity_type_enum.h"

namespace spa {
ControlFlowGraph::ControlFlowGraph(std::size_t stmt_count,
                                   std::size_t stmtlst_count,
                                   Stores stores) noexcept
        : stmt_node_index_(stmt_count + 1, 0),
          stmtlst_stmt_(stores.stmtlst_statements),
          type_store_(stores.type_statements),
          stmtlst_parent_(stores.stmtlst_parent),
          forest_(stores.forest),
          parent_store_(stores.parent_store) {
    nodes_.reserve(stmtlst_count * 3 / 2);
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
        stmt_node_index_[last] = stmt_node_index_[first_stmt];
    }
    return last;
}
void ControlFlowGraph::Precompute() noexcept {
    for (auto n = ++nodes_.begin(); n != nodes_.end(); ++n) {
        if (!n->prev.empty()) {
            next_.emplace_back(n->start);
        } else {
            roots_.emplace_back(n->start);
        }
        if (n->next.first != 0) {
            prev_.emplace_back(n->stop);
        }
        for (int i = n->start + 1; i <= n->stop; ++i) {
            prev_.emplace_back(i - 1), next_.emplace_back(i);
        }
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
bool ControlFlowGraph::IsNextT(StmtNo prev, StmtNo next) const noexcept {
    if (prev >= stmt_node_index_.size() || next >= stmt_node_index_.size())
        return false;
    StmtLstIndex prev_lst_index = stmtlst_stmt_.GetStmtlst(prev);
    StmtLstIndex next_lst_index = stmtlst_stmt_.GetStmtlst(next);
    if (!forest_.SameProcedure(prev_lst_index, next_lst_index)) return false;
    std::vector<StmtNo> stack = {prev};
    BitArray visited(stmt_node_index_.size());
    while (!stack.empty()) {
        StmtNo curr = stack.back();
        stack.pop_back();
        if (visited.Get(curr)) continue;
        visited.Set(curr);
        const auto& node = GetNode(curr);
        if (curr != node.stop && next > curr && next <= node.stop) return true;
        if (node.next.first == next) {
            return true;
        } else if (node.next.first != 0) {
            stack.emplace_back(node.next.first);
        }
        if (node.next.second == next) {
            return true;
        } else if (node.next.second != 0) {
            stack.emplace_back(node.next.second);
        }
    }
    return false;
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
std::set<StmtNo> ControlFlowGraph::GetPrevT(StmtNo next,
                                            StmtType stmt_type) const noexcept {
    std::vector<StmtNo> stack = {next};
    std::set<StmtNo> results;
    BitArray visited(stmt_node_index_.size());
    while (!stack.empty()) {
        StmtNo curr = stack.back();
        stack.pop_back();
        if (visited.Get(curr)) continue;
        visited.Set(curr);
        const auto& node = GetNode(curr);
        for (StmtNo i = curr - 1; i >= node.start; --i) {
            if (stmt_type == StmtType::kAll ||
                type_store_.GetType(i) == stmt_type)
                results.emplace_hint(results.begin(), i);
        }
        for (StmtNo s : node.prev) {
            if (stmt_type == StmtType::kAll ||
                type_store_.GetType(s) == stmt_type) {
                results.emplace_hint(results.begin(), s);
            }
            if (!visited.Get(s)) stack.emplace_back(s);
        }
    }
    return results;
}
std::set<StmtNo> ControlFlowGraph::GetNext(StmtNo prev,
                                           StmtType stmt_type) const noexcept {
    if (prev >= stmt_node_index_.size()) return {};
    return FilterPairResult(GetNextPair(prev), stmt_type);
}
std::set<StmtNo> ControlFlowGraph::GetNextT(StmtNo prev,
                                            StmtType stmt_type) const noexcept {
    std::vector<StmtNo> stack = {prev};
    auto results = GetNextTVec(prev, stmt_type);
    return {results.begin(), results.end()};
}
std::vector<StmtNo> ControlFlowGraph::GetNextTVec(
        StmtNo prev, StmtType stmt_type) const noexcept {
    std::vector<StmtNo> stack = {prev};
    std::vector<StmtNo> results;
    BitArray visited(stmt_node_index_.size());
    while (!stack.empty()) {
        StmtNo curr = stack.back();
        stack.pop_back();
        if (visited.Get(curr)) continue;
        visited.Set(curr);
        const auto& node = GetNode(curr);
        for (StmtNo i = curr + 1; i <= node.stop; ++i) {
            if (stmt_type == StmtType::kAll ||
                type_store_.GetType(i) == stmt_type)
                results.emplace_back(i);
        }
        const auto [first, second] = node.next;
        if (first != 0) {
            if (type_store_.GetType(first) == stmt_type ||
                stmt_type == StmtType::kAll)
                results.emplace_back(first);
            stack.emplace_back(first);
        }
        if (second != 0) {
            if (type_store_.GetType(second) == stmt_type ||
                stmt_type == StmtType::kAll)
                results.emplace_back(second);
            stack.back() = second;
            stack.emplace_back(first);
        }
    }
    return results;
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
std::size_t ControlFlowGraph::ToBit(std::size_t row,
                                    std::size_t col) const noexcept {
    return col * stmt_node_index_.size() + row;
}
PairVec<StmtNo> ControlFlowGraph::GetNextTPairs(StmtType prev_type,
                                                StmtType next_type) noexcept {
    if (prev_type == StmtType::kAll) {
        return next_type == StmtType::kAll
                       ? GetNextTPairsNoFilter()
                       : GetNextTPairsFilterByNext(next_type);
    }
    if (next_type == StmtType::kAll)
        return GetNextTPairsFilterByPrev(prev_type);
    return GetNextTPairsFilterBoth(prev_type, next_type);
}
PairVec<StmtNo> ControlFlowGraph::GetNextTPairsNoFilter() noexcept {
    PairVec<StmtNo> results;
    std::for_each(roots_.begin(), roots_.end(),
                  [this, &results](StmtNo stmt_no) {
                      AddTPairsInProc(stmt_no, results);
                  });
    return results;
}
void ControlFlowGraph::AddPairsWithPrevInQueue(PairVec<StmtNo>& results,
                                               BitArray& added,
                                               StmtNo next_stmt,
                                               StmtType filter_type) noexcept {
    auto& [prev, next] = results;
    for (StmtNo s : queue_.front()) {
        auto pos = ToBit(s, next_stmt);
        if (!added.Get(pos) && (filter_type == StmtType::kAll ||
                                type_store_.GetType(s) == filter_type)) {
            prev.emplace_back(s);
            added.Set(pos);
        }
    }
    next.resize(prev.size(), next_stmt);
}
void ControlFlowGraph::AppendAndQueue(PairVec<StmtNo>& results, BitArray& added,
                                      StmtNo next_stmt,
                                      StmtType type) noexcept {
    if (GetNode(next_stmt).next.first == 0) return;
    if (type_store_.GetType(next_stmt) == StmtType::kWhile) {
        const auto pos = ToBit(next_stmt, next_stmt);
        if (added.Get(pos)) return;
        added.Set(pos);
        if (type == StmtType::kAll || type == StmtType::kWhile) {
            results.first.emplace_back(next_stmt);
            results.second.emplace_back(next_stmt);
        }
    }
    queue_.emplace(queue_.front()).emplace_back(next_stmt);
}
void ControlFlowGraph::AddTPairsInProc(StmtNo root,
                                       PairVec<StmtNo>& results) noexcept {
    queue_.emplace().emplace_back(root);
    auto& [prev, next] = results;
    BitArray added(stmt_node_index_.size() * stmt_node_index_.size());
    while (!queue_.empty()) {
        const auto& node = GetNode(queue_.front().back());
        for (StmtNo i = node.start; i < node.stop; ++i) {
            queue_.front().emplace_back(i + 1);
            auto num_next = node.stop - i;
            next.resize(next.size() + num_next);
            std::iota(next.end() - num_next, next.end(), i + 1);
            std::for_each(next.end() - num_next, next.end(),
                          [this, &added, i](const auto s) {
                              added.Set(ToBit(i, s));
                          });
            prev.resize(next.size(), i);
        }
        for (StmtNo n : {node.next.first, node.next.second}) {
            if (n == 0) break;
            AppendAndQueue(results, added, n);
            AddPairsWithPrevInQueue(results, added, n);
        }
        queue_.pop();
    }
}
PairVec<StmtNo> ControlFlowGraph::GetNextTPairsFilterByNext(
        StmtType type) noexcept {
    PairVec<StmtNo> results;
    std::for_each(roots_.begin(), roots_.end(),
                  [this, &results, type](StmtNo stmt_no) {
                      AddTPairsInProcFilterNext(stmt_no, results, type);
                  });
    return results;
}
void ControlFlowGraph::AddTPairsInProcFilterNext(StmtNo root,
                                                 PairVec<StmtNo>& results,
                                                 StmtType next_type) noexcept {
    queue_.emplace().emplace_back(root);
    auto& [prev, next] = results;
    BitArray added(stmt_node_index_.size() * stmt_node_index_.size());
    while (!queue_.empty()) {
        const auto& node = GetNode(queue_.front().back());
        for (StmtNo i = node.start; i < node.stop; ++i) {
            queue_.front().emplace_back(i + 1);
            AddConsecutiveTPairsFilterNext(results, added, i, node.stop,
                                           next_type);
        }
        for (StmtNo n : {node.next.first, node.next.second}) {
            if (n == 0) break;
            AppendAndQueue(results, added, n, next_type);
            if (type_store_.GetType(n) != next_type) continue;
            AddPairsWithPrevInQueue(results, added, n);
        }
        queue_.pop();
    }
}
void ControlFlowGraph::AddConsecutiveTPairsFilterNext(
        PairVec<StmtNo>& results, BitArray& added, StmtNo start, StmtNo stop,
        StmtType next_type) noexcept {
    auto& [prev, next] = results;
    for (StmtNo i = start + 1; i <= stop; ++i) {
        if (type_store_.GetType(i) == next_type) {
            next.emplace_back(i);
            added.Set(ToBit(start, i));
        }
    }
    prev.resize(next.size(), start);
}
PairVec<StmtNo> ControlFlowGraph::GetNextTPairsFilterByPrev(
        StmtType type) noexcept {
    PairVec<StmtNo> results;
    std::for_each(roots_.begin(), roots_.end(),
                  [this, &results, type](StmtNo stmt_no) {
                      AddTPairsInProcFilterPrev(stmt_no, results, type);
                  });
    return results;
}
void ControlFlowGraph::AddTPairsInProcFilterPrev(StmtNo root,
                                                 PairVec<StmtNo>& results,
                                                 StmtType prev_type) noexcept {
    queue_.emplace().emplace_back(root);
    auto& [prev, next] = results;
    BitArray added(stmt_node_index_.size() * stmt_node_index_.size());
    while (!queue_.empty()) {
        const auto& node = GetNode(queue_.front().back());
        for (StmtNo i = node.start; i < node.stop; ++i) {
            queue_.front().emplace_back(i + 1);
            if (type_store_.GetType(i) != prev_type) continue;
            auto num_next = node.stop - i;
            next.resize(next.size() + num_next);
            std::iota(next.end() - num_next, next.end(), i + 1);
            std::for_each(next.end() - num_next, next.end(),
                          [this, &added, i](const auto s) {
                              added.Set(ToBit(i, s));
                          });
            prev.resize(next.size(), i);
        }
        for (StmtNo n : {node.next.first, node.next.second}) {
            if (n == 0) break;
            AppendAndQueue(results, added, n, prev_type);
            AddPairsWithPrevInQueue(results, added, n, prev_type);
        }
        queue_.pop();
    }
}
PairVec<StmtNo> ControlFlowGraph::GetNextTPairsFilterBoth(
        StmtType prev_type, StmtType next_type) noexcept {
    PairVec<StmtNo> results;
    auto& [prev, next] = results;
    for (StmtNo stmt_no : type_store_.GetStatements(prev_type)) {
        auto all_next = GetNextT(stmt_no, next_type);
        next.insert(next.end(), all_next.begin(), all_next.end());
        prev.resize(next.size(), stmt_no);
    }
    return results;
}
std::set<StmtNo> ControlFlowGraph::GetNextTSelf(
        StmtType stmt_type) const noexcept {
    std::set<StmtNo> results;
    BitArray added(stmt_node_index_.size());
    for (const auto while_stmt : type_store_.GetStatements(StmtType::kWhile)) {
        if (added.Get(while_stmt)) continue;
        StmtNo last_stmt = parent_store_.GetContainerLastStmt(StmtType::kWhile,
                                                              while_stmt);
        for (StmtNo i = while_stmt; i <= last_stmt; ++i) {
            StmtType i_type = type_store_.GetType(i);
            if (i_type == StmtType::kWhile) added.Set(i);
            if (stmt_type != StmtType::kAll && stmt_type != i_type) continue;
            results.emplace_hint(results.end(), i);
        }
    }
    return results;
}
}  // namespace spa
