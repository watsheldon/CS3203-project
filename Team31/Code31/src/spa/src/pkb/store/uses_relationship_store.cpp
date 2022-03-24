#include "uses_relationship_store.h"

#include <functional>
#include <vector>

#include "common/bitvec2d.h"
#include "pkb/secondary_structure/container_forest.h"
#include "stmtlst_parent_store.h"
#include "stmtlst_statements_store.h"
#include "type_statements_store.h"

namespace spa {
void UsesRelationshipStore::Set(int stmt_no, std::vector<int>&& var_indices) {
    stmt_var_.Set(stmt_no, std::forward<std::vector<int>>(var_indices));
}

const std::vector<int>& UsesRelationshipStore::GetVarIndex(int stmt_no) const {
    return stmt_var_.GetVals(stmt_no);
}

void UsesRelationshipStore::AddConditionRel(
        const TypeStatementsStore& type_statement_store,
        const ContainerInfo& info, ContainerBitmap& bitmaps) {
    auto& [if_added, while_added, proc_added] = bitmaps;
    std::array<Ref<BitVec2D>, 2> container_bitmaps{{if_added, while_added}};
    auto& if_var_pairs = stmt_var_pairs_[ConvertStmtType(StmtType::kIf)];
    auto& while_var_pairs = stmt_var_pairs_[ConvertStmtType(StmtType::kWhile)];
    std::array<Ref<PairVec<int>>, 2> container_var_pairs{
            {if_var_pairs, while_var_pairs}};
    auto& all_if_stmts = type_statement_store.GetStatements(StmtType::kIf);
    auto& all_while_stmts =
            type_statement_store.GetStatements(StmtType::kWhile);
    std::array<std::vector<int>, 2> all_container_stmts{
            {all_if_stmts, all_while_stmts}};

    for (int pos = 0; pos < 2; ++pos) {
        auto& stmt_var_pairs = container_var_pairs[pos].get();
        for (auto stmt_no : all_container_stmts[pos]) {
            auto& var_indices = GetVarIndex(stmt_no);
            AddConditionIndirectUses(stmt_no, var_indices, container_var_pairs,
                                     info, bitmaps);
            auto& bitmap = container_bitmaps[pos].get();
            AddConditionDirectUses(stmt_no, var_indices, stmt_var_pairs,
                                   bitmap);
        }
    }
}

void UsesRelationshipStore::AddAllIndirectRel(
        const TypeStatementsStore& type_statement_store,
        const ContainerInfo& info, ContainerBitmap& bitmaps) {
    FillIndirectRels(relevant_stmt_types_, type_statement_store, info, bitmaps);
}

void UsesRelationshipStore::AddAllDirectRel(const TypeStatementsStore& store) {
    FillDirectRels(relevant_stmt_types_, store);
}

// Add direct Uses of condition variables by container stmt with the given
// index.
void UsesRelationshipStore::AddConditionDirectUses(
        int stmt_no, const std::vector<int>& var_indices,
        PairVec<int>& stmt_var_pairs, BitVec2D& bitmap) {
    auto& [container_stmts, container_vars] = stmt_var_pairs;
    for (auto v : var_indices) {
        if (bitmap.At(stmt_no, v)) continue;
        container_vars.emplace_back(v);
        bitmap.Set(stmt_no, v);
    }
    container_stmts.resize(container_vars.size(), stmt_no);
}

// Add indirect Uses of condition variables by ancestors of stmt_no.
void UsesRelationshipStore::AddConditionIndirectUses(
        int stmt_no, const std::vector<int>& var_indices,
        std::array<Ref<PairVec<int>>, 2>& container_var_pairs,
        const ContainerInfo& info, ContainerBitmap& bitmaps) {
    const auto& [forest, stmtlst_parent, stmtlst_stmt] = info;
    auto& [if_added, while_added, proc_added] = bitmaps;
    auto& [if_var_pairs, while_var_pairs] = container_var_pairs;
    auto stmtlst = stmtlst_stmt.GetStmtlst(stmt_no);
    auto ancestors = forest.GetAncestryTrace(stmtlst);
    auto& [type, proc_index] = stmtlst_parent.GetParent(ancestors.back());
    for (auto v : var_indices) {
        ProcessProcedureAncestor(proc_index, v, proc_added);
    }
    ancestors.pop_back();
    for (auto a : ancestors) {
        auto& [type, index] = stmtlst_parent.GetParent(a);
        auto& [stmts, vars] = type == StmtlstParentStore::kIf
                                      ? if_var_pairs.get()
                                      : while_var_pairs.get();
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
}  // namespace spa
