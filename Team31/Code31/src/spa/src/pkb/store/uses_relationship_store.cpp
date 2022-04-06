#include "uses_relationship_store.h"

#include <cassert>
#include <vector>

#include "common/aliases.h"
#include "common/bitvec2d.h"
#include "stmtlst_parent_store.h"
#include "type_statements_store.h"

namespace spa {
UsesRelationshipStore::UsesRelationshipStore(std::size_t stmt_size,
                                             std::size_t var_size,
                                             std::size_t proc_size)
        : UsesModifiesStoreBase(stmt_size, var_size, proc_size),
          condition_direct_uses_(
                  {IndexBimap<std::set<int>>(stmt_size, var_size),
                   IndexBimap<std::set<int>>(stmt_size, var_size)}) {}
void UsesRelationshipStore::Set(StmtNo stmt_no,
                                std::vector<VarIndex>&& var_indices) {
    stmt_var_.Set(stmt_no, std::move(var_indices));
}
const std::vector<VarIndex>& UsesRelationshipStore::GetVarIndex(
        StmtNo stmt_no) const {
    return stmt_var_.GetVals(stmt_no);
}
void UsesRelationshipStore::AddConditionRel(
        const AuxiliaryData& data_store) noexcept {
    auto& [type_stmt, cont_info, bitmaps] = data_store;
    for (auto stmt_type : kContainerStmtTypes) {
        const auto& stmts = type_stmt.GetStatements(stmt_type);
        auto& bitmap = stmt_type == StmtType::kIf ? bitmaps.if_added
                                                  : bitmaps.while_added;
        for (auto stmt_no : stmts) {
            AddConditionIndirectUses(stmt_no, cont_info, bitmaps);
            auto& stmt_var_pairs = stmt_var_pairs_[StmtTypeToIndex(stmt_type)];
            AddConditionDirectUses(stmt_no, stmt_var_pairs, bitmap);
        }
    }
}
void UsesRelationshipStore::AddAllIndirectRel(
        const AuxiliaryData& data_store) noexcept {
    FillIndirectRels(relevant_stmt_types_, data_store);
}
void UsesRelationshipStore::AddAllDirectRel(
        const TypeStatementsStore& store) noexcept {
    FillDirectRels(relevant_stmt_types_, store);
}
void UsesRelationshipStore::PrecompileStep(
        const TypeStatementsStore& type_store) noexcept {
    // make a copy of the stmt_var relationships before indirect uses are added
    for (auto stmt_type : kContainerStmtTypes) {
        auto& stmt_var_map =
                condition_direct_uses_[GetCondTypeIndex(stmt_type)];
        auto& [direct_stmts, direct_vars] =
                condition_direct_pairs_[GetCondTypeIndex(stmt_type)];
        auto& stmt_set = condition_direct_stmts_[GetCondTypeIndex(stmt_type)];
        const std::vector<StmtNo>& statements =
                type_store.GetStatements(stmt_type);
        for (StmtNo stmt_no : statements) {
            const auto& vars = GetVarIndex(stmt_no);
            if (vars.empty()) continue;
            stmt_var_map.Set(stmt_no, vars);
            direct_vars.insert(direct_vars.end(), vars.begin(), vars.end());
            direct_stmts.resize(direct_vars.size(), stmt_no);
            stmt_set.emplace(stmt_no);
        }
    }
}
// Add direct Uses of condition variables by container stmt with the given
// index.
void UsesRelationshipStore::AddConditionDirectUses(
        StmtNo stmt_no, PairVec<StmtNo, VarIndex>& stmt_var_pairs,
        BitVec2D& bitmap) const {
    auto& [container_stmts, container_vars] = stmt_var_pairs;
    for (auto v : GetVarIndex(stmt_no)) {
        if (bitmap.At(stmt_no, v)) continue;
        container_vars.emplace_back(v);
        bitmap.Set(stmt_no, v);
    }
    container_stmts.resize(container_vars.size(), stmt_no);
}
// Add indirect Uses of condition variables by ancestors of stmt_no.
void UsesRelationshipStore::AddConditionIndirectUses(
        StmtNo stmt_no, const ContainerInfo& info,
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
bool UsesRelationshipStore::ExistUses(StmtNo stmt_no, VarIndex var_index) {
    return ExistRel(stmt_no, var_index);
}
std::set<VarIndex> UsesRelationshipStore::GetUses(
        StmtNo stmt_no) const noexcept {
    return GetRelRelatedVars(stmt_no);
}
std::set<StmtNo> UsesRelationshipStore::GetUses(
        VarIndex var_index, StmtType type, const TypeStatementsStore& store) {
    return GetRelRelatedVars(var_index, type, store);
}
bool UsesRelationshipStore::ExistUsesP(ProcIndex proc_index,
                                       VarIndex var_index) {
    return ExistRelP(proc_index, var_index);
}
bool UsesRelationshipStore::ExistUsesP(ProcIndex proc_index) {
    return ExistRelP(proc_index);
}
const std::set<StmtNo>& UsesRelationshipStore::GetContainers(
        StmtType type, VarIndex var_index) const {
    return condition_direct_uses_[GetCondTypeIndex(type)].GetKeys(var_index);
}
std::set<StmtNo> UsesRelationshipStore::GetContainers(StmtType type) const {
    return condition_direct_stmts_[GetCondTypeIndex(type)];
}
const PairVec<StmtNo, VarIndex>& UsesRelationshipStore::GetContainerVarPairs(
        StmtType type) const {
    return condition_direct_pairs_[GetCondTypeIndex(type)];
}
}  // namespace spa
