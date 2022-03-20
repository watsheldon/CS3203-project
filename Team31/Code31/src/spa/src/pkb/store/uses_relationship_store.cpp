#include "uses_relationship_store.h"

#include <functional>
#include <vector>

#include "common/bitvec2d.h"
#include "pkb/secondary_structure/container_forest.h"
#include "stmtlst_parent_store.h"
#include "stmtlst_statements_store.h"
#include "type_statements_store.h"

namespace spa {
template <typename T>
using Ref = std::reference_wrapper<T>;

void UsesRelationshipStore::Set(int stmt_no, std::vector<int>&& var_indices) {
    stmt_var_.Set(stmt_no, std::forward<std::vector<int>>(var_indices));
}

const std::vector<int>& UsesRelationshipStore::GetVarIndex(int stmt_no) const {
    return stmt_var_.GetVals(stmt_no);
}

void UsesRelationshipStore::AddConditionRel(
        const ContainerForest& forest, const StmtlstParentStore& stmtlst_parent,
        const StmtlstStatementsStore& stmtlst_stmt,
        const TypeStatementsStore& type_statement_store, BitVec2D& if_added,
        BitVec2D& while_added) {
    std::array<Ref<BitVec2D>, 2> container_bitmaps{{if_added, while_added}};
    auto& if_var_pairs = stmt_var_pairs_[static_cast<int>(StmtType::kIf) - 1];
    auto& while_var_pairs =
            stmt_var_pairs_[static_cast<int>(StmtType::kWhile) - 1];
    std::array<Ref<PairVec<int>>, 2> container_var_pairs{
            {if_var_pairs, while_var_pairs}};
    auto& all_if_stmts = type_statement_store.GetStatements(StmtType::kIf);
    auto& all_while_stmts =
            type_statement_store.GetStatements(StmtType::kWhile);
    std::array<std::vector<int>, 2> all_container_stmts{
            {all_if_stmts, all_while_stmts}};

    for (int pos = 0; pos < 2; ++pos) {
        auto& [container_stmts, container_vars] =
                container_var_pairs[pos].get();
        for (auto i : all_container_stmts[pos]) {
            // Add indirect Uses of condition variables by ancestors of i.
            auto& var_indices = GetVarIndex(i);
            auto stmtlst = stmtlst_stmt.GetStmtlst(i);
            auto ancestors = forest.GetAncestryTrace(stmtlst);
            for (auto a : ancestors) {
                auto& [type, index] = stmtlst_parent.GetParent(a);
                if (type == StmtlstParentStore::kProc) break;
                auto& [stmts, vars] = type == StmtlstParentStore::kIf
                                              ? if_var_pairs
                                              : while_var_pairs;
                auto& added = type == StmtlstParentStore::kIf ? if_added
                                                              : while_added;
                for (auto v : var_indices) {
                    if (added.At(a, v)) continue;
                    vars.emplace_back(v);
                    added.Set(a, v);
                }
                if (stmts.size() == vars.size()) break;
                stmts.resize(vars.size(), index);
            }
            // Add direct Uses of condition variables by container statement i.
            auto& bitmap = container_bitmaps[pos].get();
            for (auto v : var_indices) {
                if (bitmap.At(i, v)) continue;
                container_vars.emplace_back(v);
                bitmap.Set(i, v);
            }
            container_stmts.resize(container_vars.size(), i);
        }
    }
}

void UsesRelationshipStore::InitIndirectTypes(
        const TypeStatementsStore& type_statement_store,
        const StmtlstStatementsStore& stmtlst_stmt,
        const StmtlstParentStore& stmtlst_parent, const ContainerForest& forest,
        BitVec2D& if_added, BitVec2D& while_added) {
    InitIndirectRel(
            std::array<StmtType, 2>{StmtType::kAssign, StmtType::kPrint},
            type_statement_store, stmtlst_stmt, stmtlst_parent, forest,
            if_added, while_added);
}

void UsesRelationshipStore::InitDirectTypes(const TypeStatementsStore& store) {
    InitDirectRel(std::array<StmtType, 2>{StmtType::kAssign, StmtType::kPrint},
                  store);
}
}  // namespace spa
