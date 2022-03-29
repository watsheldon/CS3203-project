#include "uses_relationship_store.h"

#include <cassert>
#include <vector>

#include "common/bitvec2d.h"
#include "stmtlst_parent_store.h"
#include "type_statements_store.h"

namespace spa {
UsesRelationshipStore::UsesRelationshipStore(std::size_t stmt_size,
                      std::size_t var_size,
                      std::size_t proc_size): UsesModifiesStoreBase(stmt_size,var_size, proc_size), condition_direct_uses_(stmt_size, var_size){}
void UsesRelationshipStore::Set(int stmt_no, std::vector<int>&& var_indices) {
    stmt_var_.Set(stmt_no, std::forward<std::vector<int>>(var_indices));
}
const std::vector<int>& UsesRelationshipStore::GetVarIndex(int stmt_no) const {
    return stmt_var_.GetVals(stmt_no);
}
void UsesRelationshipStore::AddConditionRel(const AuxiliaryData& data_store) {
    auto& [type_stmt, cont_info, bitmaps] = data_store;
    for (auto stmt_type : kContainerStmtTypes) {
        const auto& stmts = type_stmt.GetStatements(stmt_type);
        auto& bitmap = stmt_type == StmtType::kIf ? bitmaps.if_added
                                                  : bitmaps.while_added;
        for (auto stmt_no : stmts) {
            AddConditionIndirectUses(stmt_no, cont_info, bitmaps);
            auto& stmt_var_pairs = stmt_var_pairs_[StmtTypeToIndex(stmt_type)];
            AddConditionDirectUses(stmt_type, stmt_no, stmt_var_pairs, bitmap);
        }
    }
}
void UsesRelationshipStore::AddAllIndirectRel(const AuxiliaryData& data_store) {
    FillIndirectRels(relevant_stmt_types_, data_store);
}
void UsesRelationshipStore::AddAllDirectRel(const TypeStatementsStore& store) {
    FillDirectRels(relevant_stmt_types_, store);
}
// Add direct Uses of condition variables by container stmt with the given
// index.
void UsesRelationshipStore::AddConditionDirectUses(StmtType type, int stmt_no,
                                                   PairVec<int>& stmt_var_pairs,
                                                   BitVec2D& bitmap) {
    auto& [container_stmts, container_vars] = stmt_var_pairs;
    const auto & condition_vars = GetVarIndex(stmt_no);
    auto& [condition_direct_stmts, condition_direct_vars] = condition_direct_pairs_[GetIndex(type)];
    condition_direct_uses_.Set(stmt_no, condition_vars);
    for (auto v : condition_vars) {
        condition_direct_vars.emplace_back(v);
        if (bitmap.At(stmt_no, v)) continue;
        container_vars.emplace_back(v);
        bitmap.Set(stmt_no, v);
    }
    condition_direct_stmts.resize(condition_direct_vars.size(), stmt_no);
    container_stmts.resize(container_vars.size(), stmt_no);
}
// Add indirect Uses of condition variables by ancestors of stmt_no.
void UsesRelationshipStore::AddConditionIndirectUses(
        int stmt_no, const ContainerInfo& info,
        ContainerAddedTrackers& bitmaps) {
    const auto& [forest, stmtlst_parent, stmtlst_stmt] = info;
    auto& [if_added, while_added, proc_added] = bitmaps;
    auto stmtlst = stmtlst_stmt.GetStmtlst(stmt_no);
    auto ancestors = forest.GetAncestryTrace(stmtlst);
    const auto& proc_index = stmtlst_parent.GetParent(ancestors.back()).index;
    auto& var_indices = GetVarIndex(stmt_no);
    for (auto v : var_indices) {
        ProcessProcedureAncestor(proc_index, v, proc_added);
    }
    ancestors.pop_back();
    for (auto a : ancestors) {
        auto& [type, index] = stmtlst_parent.GetParent(a);
        auto& [stmts, vars] =
                type == StmtlstParentStore::kIf
                        ? stmt_var_pairs_[StmtTypeToIndex(StmtType::kIf)]
                        : stmt_var_pairs_[StmtTypeToIndex(StmtType::kWhile)];
        auto& added = type == StmtlstParentStore::kIf ? if_added : while_added;
        for (auto v : var_indices) {
            if (added.At(a, v)) break;
            vars.emplace_back(v);
            added.Set(a, v);
        }
        if (stmts.size() == vars.size()) break;
        stmts.resize(vars.size(), index);
    }
}
bool UsesRelationshipStore::ExistUses(int stmt_no, int var_index) {
    return ExistRel(stmt_no, var_index);
}
std::set<int> UsesRelationshipStore::GetUses(int stmt_no) {
    return GetRelRelatedVars(stmt_no);
}
std::set<int> UsesRelationshipStore::GetUses(int var_index, StmtType type,
                                             const TypeStatementsStore& store) {
    return GetRelRelatedVars(var_index, type, store);
}
bool UsesRelationshipStore::ExistUsesP(int proc_index, int var_index) {
    return ExistRelP(proc_index, var_index);
}
bool UsesRelationshipStore::ExistUsesP(int proc_index) {
    return ExistRelP(proc_index);
}
const std::set<int>& UsesRelationshipStore::GetContainers(StmtType type,
                                                          int var_index) const{
    return condition_direct_uses_.GetKeys(var_index);
}
std::set<int> UsesRelationshipStore::GetContainers(StmtType type) const {
    auto containers = condition_direct_pairs_[GetIndex(type)].first;
    return {containers.begin(), containers.end()};
}
const PairVec<int>& UsesRelationshipStore::GetContainerVarPairs(
        StmtType type) const{
    return condition_direct_pairs_[GetIndex(type)];
}
}  // namespace spa
