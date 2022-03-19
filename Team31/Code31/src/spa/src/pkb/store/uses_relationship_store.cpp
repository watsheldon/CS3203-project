#include "uses_relationship_store.h"

#include <cassert>
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

void UsesRelationshipStore::Compile(
        const TypeStatementsStore& type_statement_store,
        const ContainerForest& forest, const StmtlstParentStore& stmtlst_parent,
        const StmtlstStatementsStore& stmtlst_stmt) {
    FillVars();

    std::array<StmtType, 2> direct_stmt_types{
            {StmtType::kAssign, StmtType::kPrint}};
    for (const auto& stmt_type : direct_stmt_types) {
        auto& stmt_var_pairs =
                stmt_var_pairs_.at(static_cast<int>(stmt_type) - 1);
        AddDirectRel(stmt_var_pairs,
                     type_statement_store.GetStatements(stmt_type));
    }

    AddContainerRel(forest, stmtlst_parent, stmtlst_stmt, type_statement_store);
}

// Add indirect Uses for container statements.
void UsesRelationshipStore::AddContainerRel(
        const ContainerForest& forest, const StmtlstParentStore& stmtlst_parent,
        const StmtlstStatementsStore& stmtlst_stmt,
        const TypeStatementsStore& type_statement_store) {
    BitVec2D if_added(num_stmts + 1, num_vars + 1);
    BitVec2D while_added(num_stmts + 1, num_vars + 1);
    auto& if_var_pairs_ =
            stmt_var_pairs_.at(static_cast<int>(StmtType::kIf) - 1);
    auto& while_var_pairs_ =
            stmt_var_pairs_.at(static_cast<int>(StmtType::kWhile) - 1);
    auto& [if_stmts, if_vars] = if_var_pairs_;
    for (auto i : type_statement_store.GetStatements(StmtType::kIf)) {
        // Add indirect Uses of condition variables by ancestors of i.
        auto& var_indices = GetVarIndex(i);
        auto stmtlst = stmtlst_stmt.GetStmtlst(i);
        auto ancestors = forest.GetAncestryTrace(stmtlst);
        for (auto a : ancestors) {
            auto& [type, index] = stmtlst_parent.GetParent(a);
            if (type == StmtlstParentStore::kProc) break;
            auto& [stmts, vars] = type == StmtlstParentStore::kIf
                                          ? if_var_pairs_
                                          : while_var_pairs_;
            auto& added =
                    type == StmtlstParentStore::kIf ? if_added : while_added;
            for (auto v : var_indices) {
                if (added.At(a, v)) continue;
                vars.emplace_back(v);
                added.Set(a, v);
            }
            if (stmts.size() == vars.size()) break;
            stmts.resize(vars.size(), index);
        }
        // Add direct Uses of condition variables by if statement i.
        for (auto v : var_indices) {
            if (if_added.At(i, v)) continue;
            if_vars.emplace_back(v);
            if_added.Set(i, v);
        }
        if_stmts.resize(if_vars.size(), i);
    }
    auto& [while_stmts, while_vars] = while_var_pairs_;
    for (auto i : type_statement_store.GetStatements(StmtType::kWhile)) {
        // Add indirect Uses of condition variables by ancestors of i.
        auto& var_indices = GetVarIndex(i);
        auto stmtlst = stmtlst_stmt.GetStmtlst(i);
        auto ancestors = forest.GetAncestryTrace(stmtlst);
        for (auto a : ancestors) {
            auto& [type, index] = stmtlst_parent.GetParent(a);
            if (type == StmtlstParentStore::kProc) break;
            auto& [stmts, vars] = type == StmtlstParentStore::kIf
                                          ? if_var_pairs_
                                          : while_var_pairs_;
            auto& added =
                    type == StmtlstParentStore::kIf ? if_added : while_added;
            for (auto v : var_indices) {
                if (added.At(a, v)) continue;
                vars.emplace_back(v);
                added.Set(a, v);
            }
            if (stmts.size() == vars.size()) break;
            stmts.resize(vars.size(), index);
        }
        // Add direct Uses of condition variables by while statement i.
        for (auto v : var_indices) {
            if (while_added.At(i, v)) continue;
            while_vars.emplace_back(v);
            while_added.Set(i, v);
        }
        while_stmts.resize(while_vars.size(), i);
    }

    std::array<StmtType, 2> indirect_stmt_types{{
            StmtType::kAssign,
            StmtType::kPrint,
    }};
    for (const auto& stmt_type : indirect_stmt_types) {
        auto& stmt_var_pairs =
                stmt_var_pairs_.at(static_cast<int>(stmt_type) - 1);
        AddIndirectRel(stmt_var_pairs, stmtlst_stmt, stmtlst_parent, forest,
                       if_added, while_added);
    }

    FillStmts();
    FillRels();
}
}  // namespace spa
