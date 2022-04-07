#include "next_calculator.h"

#include <algorithm>
#include <cassert>
#include <numeric>

#include "common/aliases.h"
#include "common/entity_type_enum.h"

namespace spa {

NextCalculator::NextCalculator(NextCalculator::Stores stores) noexcept
        : stmtlst_stmt_(stores.stmtlst_statements),
          type_store_(stores.type_statements),
          forest_(stores.forest),
          parent_store_(stores.parent_store),
          cfg_(stores.cfg) {}
bool NextCalculator::HasNext() const noexcept {
    static const bool kResult = std::any_of(
            cfg_.nodes.begin(), cfg_.nodes.end(), [](const auto& n) {
                return n.start != n.stop || n.next.first != 0;
            });
    return kResult;
}
PairOf<StmtNo> NextCalculator::GetNextPair(StmtNo prev) const noexcept {
    if (prev >= cfg_.stmt_node_index.size()) return {0, 0};
    const auto& curr_node = cfg_.GetNode(prev);
    if (prev == curr_node.stop) {
        return curr_node.next;
    }
    assert(curr_node.start < curr_node.stop);
    return {prev + 1, 0};
}
bool NextCalculator::HasNext(StmtNo prev) const noexcept {
    return GetNextPair(prev).first != 0;
}
bool NextCalculator::HasPrev(StmtNo next) const noexcept {
    if (next >= cfg_.stmt_node_index.size()) return false;
    const auto& node = cfg_.GetNode(next);
    return next != node.start || !node.prev.empty();
}
bool NextCalculator::IsNext(StmtNo prev, StmtNo next) const noexcept {
    if (prev >= cfg_.stmt_node_index.size() ||
        next >= cfg_.stmt_node_index.size())
        return false;
    const auto& prev_node = cfg_.GetNode(prev);
    return prev != prev_node.stop ? prev + 1 == next
                                  : prev_node.next.first == next ||
                                            prev_node.next.second == next;
}
bool NextCalculator::IsNextT(StmtNo prev, StmtNo next) const noexcept {
    if (prev >= cfg_.stmt_node_index.size() ||
        next >= cfg_.stmt_node_index.size())
        return false;
    StmtLstIndex prev_lst_index = stmtlst_stmt_.GetStmtlst(prev);
    StmtLstIndex next_lst_index = stmtlst_stmt_.GetStmtlst(next);
    if (!forest_.SameProcedure(prev_lst_index, next_lst_index)) return false;
    std::vector<StmtNo> stack = {prev};
    BitArray visited(cfg_.stmt_node_index.size());
    while (!stack.empty()) {
        StmtNo curr = stack.back();
        stack.pop_back();
        if (visited.Get(curr)) continue;
        visited.Set(curr);
        const auto& node = cfg_.GetNode(curr);
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
std::set<StmtNo> NextCalculator::GetPrev(StmtNo next,
                                         StmtType stmt_type) const noexcept {
    if (next >= cfg_.stmt_node_index.size()) return {};
    const auto& node = cfg_.GetNode(next);
    if (next != node.start) {
        if (stmt_type == StmtType::kAll ||
            type_store_.GetType(next - 1) == stmt_type) {
            return {next - 1};
        } else {
            return {};
        }
    }
    return FilterVecResult(node.prev, stmt_type);
}
std::set<StmtNo> NextCalculator::GetPrevT(StmtNo next,
                                          StmtType stmt_type) const noexcept {
    auto results = GetPrevTVec(next, stmt_type);
    return {results.begin(), results.end()};
}
std::vector<StmtNo> NextCalculator::GetPrevTVec(
        StmtNo next, StmtType stmt_type) const noexcept {
    std::vector<StmtNo> stack = {next};
    std::vector<StmtNo> results;
    BitArray visited(cfg_.stmt_node_index.size());
    while (!stack.empty()) {
        StmtNo curr = stack.back();
        stack.pop_back();
        if (visited.Get(curr)) continue;
        visited.Set(curr);
        const auto& node = cfg_.GetNode(curr);
        for (StmtNo i = curr - 1; i >= node.start; --i) {
            if (stmt_type == StmtType::kAll ||
                type_store_.GetType(i) == stmt_type)
                results.emplace_back(i);
        }
        for (StmtNo s : node.prev) {
            if (stmt_type == StmtType::kAll ||
                type_store_.GetType(s) == stmt_type) {
                results.emplace_back(s);
            }
            if (!visited.Get(s)) stack.emplace_back(s);
        }
    }
    return results;
}
std::set<StmtNo> NextCalculator::GetNext(StmtNo prev,
                                         StmtType stmt_type) const noexcept {
    if (prev >= cfg_.stmt_node_index.size()) return {};
    return FilterPairResult(GetNextPair(prev), stmt_type);
}
std::set<StmtNo> NextCalculator::GetNextT(StmtNo prev,
                                          StmtType stmt_type) const noexcept {
    std::vector<StmtNo> stack = {prev};
    auto results = GetNextTVec(prev, stmt_type);
    return {results.begin(), results.end()};
}
std::vector<StmtNo> NextCalculator::GetNextTVec(
        StmtNo prev, StmtType stmt_type) const noexcept {
    std::vector<StmtNo> stack = {prev};
    std::vector<StmtNo> results;
    BitArray visited(cfg_.stmt_node_index.size());
    while (!stack.empty()) {
        StmtNo curr = stack.back();
        stack.pop_back();
        if (visited.Get(curr)) continue;
        visited.Set(curr);
        const auto& node = cfg_.GetNode(curr);
        for (StmtNo i = curr + 1; i <= node.stop; ++i) {
            if (stmt_type == StmtType::kAll ||
                type_store_.GetType(i) == stmt_type)
                results.emplace_back(i);
        }
        const auto [first, second] = node.next;
        if (first != 0) {
            if (stmt_type == StmtType::kAll ||
                type_store_.GetType(first) == stmt_type)
                results.emplace_back(first);
            stack.emplace_back(first);
        }
        if (second != 0) {
            if (stmt_type == StmtType::kAll ||
                type_store_.GetType(second) == stmt_type)
                results.emplace_back(second);
            stack.back() = second;
            stack.emplace_back(first);
        }
    }
    return results;
}
std::set<StmtNo> NextCalculator::FilterPairResult(
        const PairOf<StmtNo>& pair, StmtType stmt_type) const noexcept {
    auto [first, second] = pair;
    std::set<StmtNo> ret;
    AddIfMatches(stmt_type, first, ret);
    AddIfMatches(stmt_type, second, ret);
    return ret;
}
void NextCalculator::AddIfMatches(const StmtType stmt_type,
                                  const StmtNo stmt_no,
                                  std::set<StmtNo>& results) const {
    if (stmt_no != 0 && (stmt_type == StmtType::kAll ||
                         type_store_.GetType(stmt_no) == stmt_type))
        results.emplace(stmt_no);
}
std::set<StmtNo> NextCalculator::FilterVecResult(
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
std::set<StmtNo> NextCalculator::GetNextSingleArg(
        ArgPos return_pos, StmtType stmt_type) const noexcept {
    const auto& column = return_pos == ArgPos::kFirst ? cfg_.prev : cfg_.next;
    if (stmt_type == StmtType::kAll) return {column.begin(), column.end()};
    std::set<StmtNo> results;
    for (StmtNo s : column) {
        if (type_store_.GetType(s) == stmt_type) {
            results.emplace_hint(results.end(), s);
        }
    }
    return results;
}
PairVec<StmtNo> NextCalculator::GetNextPairs(
        StmtType prev_type, StmtType next_type) const noexcept {
    if (prev_type == StmtType::kAll) {
        return next_type == StmtType::kAll
                       ? GetNextPairsNoFilter()
                       : GetNextPairsFilterByNext(next_type);
    }
    if (next_type == StmtType::kAll) return GetNextPairsFilterByPrev(prev_type);
    return GetNextPairsFilterBoth(prev_type, next_type);
}
PairVec<StmtNo> NextCalculator::GetNextPairsNoFilter() const noexcept {
    PairVec<StmtNo> results;
    auto& [prev, next] = results;
    for (const auto& node : cfg_.nodes) {
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
PairVec<StmtNo> NextCalculator::GetNextPairsFilterByNext(
        StmtType type) const noexcept {
    PairVec<StmtNo> results;
    auto& [prev, next] = results;
    for (StmtNo n : type_store_.GetStatements(type)) {
        const auto& node = cfg_.GetNode(n);
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
PairVec<StmtNo> NextCalculator::GetNextPairsFilterByPrev(StmtType type) const {
    PairVec<StmtNo> results;
    auto& [prev, next] = results;
    for (StmtNo p : type_store_.GetStatements(type)) {
        const auto& node = cfg_.GetNode(p);
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
PairVec<StmtNo> NextCalculator::GetNextPairsFilterBoth(
        StmtType prev_type, StmtType next_type) const {
    PairVec<StmtNo> results;
    auto& [prev, next] = results;
    for (StmtNo p : type_store_.GetStatements(prev_type)) {
        const auto& node = cfg_.GetNode(p);
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
PairVec<StmtNo> NextCalculator::GetNextTPairs(StmtType prev_type,
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
PairVec<StmtNo> NextCalculator::GetNextTPairsNoFilter() noexcept {
    PairVec<StmtNo> results;
    auto& [prev, next] = results;
    for (StmtNo i = 1; i < cfg_.stmt_node_index.size(); ++i) {
        auto next_stmts = GetNextTVec(i, StmtType::kAll);
        next.insert(next.end(), next_stmts.begin(), next_stmts.end());
        prev.resize(next.size(), i);
    }
    return results;
}
PairVec<StmtNo> NextCalculator::GetNextTPairsFilterByNext(
        StmtType type) noexcept {
    PairVec<StmtNo> results;
    auto& [prev, next] = results;
    for (const StmtNo i : type_store_.GetStatements(type)) {
        auto prev_stmts = GetPrevTVec(i, StmtType::kAll);
        prev.insert(prev.end(), prev_stmts.begin(), prev_stmts.end());
        next.resize(prev.size(), i);
    }
    return results;
}
PairVec<StmtNo> NextCalculator::GetNextTPairsFilterByPrev(
        StmtType type) noexcept {
    PairVec<StmtNo> results;
    auto& [prev, next] = results;
    for (const StmtNo i : type_store_.GetStatements(type)) {
        auto next_stmts = GetNextTVec(i, StmtType::kAll);
        next.insert(next.end(), next_stmts.begin(), next_stmts.end());
        prev.resize(next.size(), i);
    }
    return results;
}
PairVec<StmtNo> NextCalculator::GetNextTPairsFilterBoth(
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
std::set<StmtNo> NextCalculator::GetNextTSelf(
        StmtType stmt_type) const noexcept {
    std::set<StmtNo> results;
    BitArray added(cfg_.stmt_node_index.size());
    for (const StmtNo while_stmt :
         type_store_.GetStatements(StmtType::kWhile)) {
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
