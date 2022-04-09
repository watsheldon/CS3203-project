#include "affects_calculator.h"

#include <iterator>
#include <queue>
#include <set>
#include <stack>
#include <vector>

#include "common/aliases.h"
#include "common/bit_array.h"
#include "common/entity_type_enum.h"

namespace spa {
AffectsCalculator::AffectsCalculator(Stores stores) noexcept
        : stmtlst_stmt_(stores.stmtlst_statements),
          type_store_(stores.type_statements),
          forest_(stores.forest),
          modifies_store_(stores.modifies_store),
          uses_store_(stores.uses_store),
          cfg_(stores.cfg),
          next_(stores.next),
          stmt_count_(stores.stmt_count),
          affects_cache_(stmt_count_ + 1),
          assign_stmts_(
                  stores.type_statements.GetStatements(StmtType::kAssign)) {}
bool AffectsCalculator::ExistAffects(StmtNo first_assign,
                                     StmtNo second_assign) noexcept {
    auto indicator = affects_cache_.Get(first_assign, second_assign);
    if (indicator != Cache::Indicator::kUncalculated) {
        return indicator == Cache::Indicator::kTrue;
    }
    StmtType first_type = type_store_.GetType(first_assign);
    StmtType second_type = type_store_.GetType(second_assign);
    if (first_type != StmtType::kAssign || second_type != StmtType::kAssign) {
        affects_cache_.Set(first_assign, second_assign,
                           Cache::Indicator::kFalse);
        return false;
    }
    if (!IsSameProcedure(first_assign, second_assign)) {
        affects_cache_.Set(first_assign, second_assign,
                           Cache::Indicator::kFalse);
        return false;
    }
    VarIndex modified_var = modifies_store_.GetModifiesSingleVar(first_assign);
    if (!uses_store_.ExistUses(second_assign, modified_var)) {
        affects_cache_.Set(first_assign, second_assign,
                           Cache::Indicator::kFalse);
        return false;
    }

    if (ExistUnmodifiedPath(first_assign, second_assign, modified_var)) {
        affects_cache_.Set(first_assign, second_assign,
                           Cache::Indicator::kTrue);
        return true;
    }
    affects_cache_.Set(first_assign, second_assign, Cache::Indicator::kFalse);
    return false;
}
bool AffectsCalculator::ExistAffectsT(StmtNo first_assign,
                                      StmtNo second_assign) noexcept {
    /*
     * DFS on the AffectsGraph.
     * AffectsGraph:
     *    - Vertices: all assign stmts
     *    - Edges: (a1, a2) is an edge iff Affects(a1, a2) holds, even if a1=a2
     */
    std::stack<StmtNo, std::vector<StmtNo>> s;
    BitArray visited(assign_stmts_.size());
    visited.Set(first_assign);
    s.emplace(first_assign);
    while (!s.empty()) {
        StmtNo curr = s.top();
        s.pop();
        if (ExistAffects(curr, second_assign)) {
            return true;
        }
        std::set<StmtNo> children = GetAffected(curr);
        AddChildrenAffectsT(children, visited, s);
    }
    return false;
}
bool AffectsCalculator::HasAffected(StmtNo first_assign) noexcept {
    for (const StmtNo stmt : assign_stmts_) {
        if (ExistAffects(first_assign, stmt)) {
            return true;
        }
    }
    return false;
}
bool AffectsCalculator::HasAffecter(StmtNo second_assign) noexcept {
    for (const StmtNo stmt : assign_stmts_) {
        if (ExistAffects(stmt, second_assign)) {
            return true;
        }
    }
    return false;
}
bool AffectsCalculator::ExistAffects() noexcept {
    for (const StmtNo stmt : assign_stmts_) {
        if (HasAffected(stmt) || HasAffecter(stmt)) {
            return true;
        }
    }
    return false;
}
std::set<StmtNo> AffectsCalculator::GetAffects(ArgPos return_pos) noexcept {
    std::vector<StmtNo> results;
    for (const StmtNo stmt : assign_stmts_) {
        bool is_satisfied = return_pos == ArgPos::kFirst ? HasAffected(stmt)
                                                         : HasAffecter(stmt);
        if (is_satisfied) {
            results.emplace_back(stmt);
        }
    }
    return {results.begin(), results.end()};
}
std::set<StmtNo> AffectsCalculator::GetAffected(StmtNo first_assign) noexcept {
    std::vector<StmtNo> results;
    for (const StmtNo stmt : assign_stmts_) {
        if (ExistAffects(first_assign, stmt)) {
            results.emplace_back(stmt);
        }
    }
    return {results.begin(), results.end()};
}
std::set<StmtNo> AffectsCalculator::GetAffectedT(StmtNo first_assign) noexcept {
    std::vector<StmtNo> results;
    for (const StmtNo stmt : assign_stmts_) {
        if (ExistAffectsT(first_assign, stmt)) {
            results.emplace_back(stmt);
        }
    }
    return {results.begin(), results.end()};
}
std::set<StmtNo> AffectsCalculator::GetAffecter(StmtNo second_assign) noexcept {
    std::vector<StmtNo> results;
    for (const StmtNo stmt : assign_stmts_) {
        if (ExistAffects(stmt, second_assign)) {
            results.emplace_back(stmt);
        }
    }
    return {results.begin(), results.end()};
}
std::set<StmtNo> AffectsCalculator::GetAffecterT(
        StmtNo second_assign) noexcept {
    std::vector<StmtNo> results;
    for (const StmtNo stmt : assign_stmts_) {
        if (ExistAffectsT(stmt, second_assign)) {
            results.emplace_back(stmt);
        }
    }
    return {results.begin(), results.end()};
}
PairVec<StmtNo> AffectsCalculator::GetAffectsPairs() noexcept {
    std::vector<StmtNo> affecter;
    std::vector<StmtNo> affected;
    for (const StmtNo stmt : assign_stmts_) {
        affecter.emplace_back(stmt);
        std::set<StmtNo> affected_stmts = GetAffected(stmt);
        std::copy(affected_stmts.begin(), affected_stmts.end(),
                  std::back_inserter(affected));
        affecter.resize(affected.size(), stmt);
    }

    return {affecter, affected};
}
PairVec<StmtNo> AffectsCalculator::GetAffectsTPairs() noexcept {
    std::vector<StmtNo> affecter;
    std::vector<StmtNo> affected;
    for (const StmtNo stmt : assign_stmts_) {
        affecter.emplace_back(stmt);
        std::set<StmtNo> affected_stmts = GetAffectedT(stmt);
        std::copy(affected_stmts.begin(), affected_stmts.end(),
                  std::back_inserter(affected));
        affecter.resize(affected.size(), stmt);
    }
    return {affecter, affected};
}
bool AffectsCalculator::IsSameProcedure(StmtNo first_assign,
                                        StmtNo second_assign) const noexcept {
    StmtLstIndex first_stmtlst = stmtlst_stmt_.GetStmtlst(first_assign);
    StmtLstIndex second_stmtlst = stmtlst_stmt_.GetStmtlst(second_assign);
    return forest_.SameProcedure(first_stmtlst, second_stmtlst);
}
bool AffectsCalculator::ExistUnmodifiedPath(StmtNo first_assign,
                                            StmtNo second_assign,
                                            VarIndex var) noexcept {
    // Basically BFS on CFG.
    // The case Affects(a, a) requires some special handling.
    std::queue<StmtNo> q;
    BitArray visited(cfg_.stmt_node_index.size());
    bool is_first_node = true;
    if (first_assign != second_assign) {
        visited.Set(first_assign);
    }
    q.push(first_assign);
    while (!q.empty()) {
        StmtNo curr = q.front();
        q.pop();
        if (curr == second_assign && !is_first_node) {
            return true;
        }
        is_first_node = false;
        StmtType type = type_store_.GetType(curr);
        bool is_related_type = type == StmtType::kAssign ||
                               type == StmtType::kRead ||
                               type == StmtType::kCall;
        if (curr != first_assign && is_related_type &&
            modifies_store_.ExistModifies(curr, var)) {
            continue;
        }
        std::set<StmtNo> children = next_.GetNext(curr, StmtType::kAll);
        AddChildrenAffects(children, visited, q);
    }
    return false;
}
void AffectsCalculator::AddChildrenAffects(const std::set<StmtNo>& children,
                                           BitArray& visited,
                                           std::queue<StmtNo>& q) noexcept {
    for (const StmtNo child : children) {
        if (child != 0 && !visited.Get(child)) {
            visited.Set(child);
            q.push(child);
        }
    }
}
void AffectsCalculator::AddChildrenAffectsT(
        const std::set<StmtNo>& children, BitArray& visited,
        std::stack<StmtNo, std::vector<StmtNo>>& s) noexcept {
    for (const StmtNo stmt : children) {
        if (!visited.Get(stmt)) {
            s.emplace(stmt);
        }
    }
}
}  // namespace spa
