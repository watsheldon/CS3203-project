#include "affects_calculator.h"

#include <queue>
#include <set>

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
          next_(stores.next) {}
bool AffectsCalculator::ExistAffects(StmtNo first_assign,
                                     StmtNo second_assign) const noexcept {
    StmtType first_type = type_store_.GetType(first_assign);
    StmtType second_type = type_store_.GetType(second_assign);
    if (first_type != StmtType::kAssign || second_type != StmtType::kAssign) {
        return false;
    }
    if (!IsSameProcedure(first_assign, second_assign)) {
        return false;
    }
    VarIndex modified_var = *modifies_store_.GetModifies(first_assign).begin();
    if (!uses_store_.ExistUses(second_assign, modified_var)) {
        return false;
    }
    return ExistUnmodifiedPath(first_assign, second_assign, modified_var);
}
bool AffectsCalculator::ExistAffectsT(StmtNo first_assign,
                                      StmtNo second_assign) const noexcept {
    return false;
}
bool AffectsCalculator::HasAffected(StmtNo first_assign) const noexcept {
    return false;
}
bool AffectsCalculator::HasAffecter(StmtNo second_assign) const noexcept {
    return false;
}
bool AffectsCalculator::ExistAffects() const noexcept { return false; }
std::set<StmtNo> AffectsCalculator::GetAffects(
        ArgPos return_pos) const noexcept {
    return std::set<StmtNo>();
}
std::set<StmtNo> AffectsCalculator::GetAffected(
        StmtNo first_assign) const noexcept {
    return std::set<StmtNo>();
}
std::set<StmtNo> AffectsCalculator::GetAffectedT(
        StmtNo first_assign) const noexcept {
    return std::set<StmtNo>();
}
std::set<StmtNo> AffectsCalculator::GetAffecter(
        StmtNo second_assign) const noexcept {
    return std::set<StmtNo>();
}
std::set<StmtNo> AffectsCalculator::GetAffecterT(
        StmtNo second_assign) const noexcept {
    return std::set<StmtNo>();
}
PairVec<StmtNo> AffectsCalculator::GetAffectsPairs() const noexcept {
    return spa::PairVec<StmtNo>();
}
PairVec<StmtNo> AffectsCalculator::GetAffectsTPairs() const noexcept {
    return spa::PairVec<StmtNo>();
}
bool AffectsCalculator::IsSameProcedure(StmtNo first_assign,
                                        StmtNo second_assign) const noexcept {
    StmtLstIndex first_stmtlst = stmtlst_stmt_.GetStmtlst(first_assign);
    StmtLstIndex second_stmtlst = stmtlst_stmt_.GetStmtlst(second_assign);
    return forest_.SameProcedure(first_stmtlst, second_stmtlst);
}
bool AffectsCalculator::ExistUnmodifiedPath(StmtNo first_assign,
                                            StmtNo second_assign,
                                            VarIndex var) const noexcept {
    // BFS on CFG
    if (first_assign == second_assign) {
        return next_.IsNextT(first_assign, second_assign);
    }
    std::queue<StmtNo> q;
    BitArray visited(cfg_.stmt_node_index.size());
    visited.Set(first_assign);
    q.push(first_assign);
    while (!q.empty()) {
        StmtNo curr = q.front();
        q.pop();
        if (curr == second_assign) {
            return true;
        }
        StmtType type = type_store_.GetType(curr);
        bool is_related_type = type == StmtType::kAssign ||
                               type == StmtType::kRead ||
                               type == StmtType::kCall;
        if (curr != first_assign && is_related_type &&
            modifies_store_.ExistModifies(curr, var)) {
            continue;
        }
        const auto& node = cfg_.GetNode(curr);
        StmtNo first_child = node.next.first;
        StmtNo second_child = node.next.second;
        if (first_child != 0 && !visited.Get(first_child)) {
            visited.Set(first_child);
            q.push(first_child);
        }
        if (second_child != 0 && !visited.Get(second_child)) {
            visited.Set(second_child);
            q.push(second_child);
        }
    }
    return false;
}
}  // namespace spa
