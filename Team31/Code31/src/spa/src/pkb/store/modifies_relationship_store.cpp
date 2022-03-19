#include "modifies_relationship_store.h"

#include <cassert>
#include <vector>

#include "common/bitvec2d.h"
#include "pkb/secondary_structure/container_forest.h"
#include "stmtlst_parent_store.h"
#include "stmtlst_statements_store.h"
#include "type_statements_store.h"

namespace spa {
void ModifiesRelationshipStore::Set(int stmt_no, int var_index) {
    stmt_var_.Set(stmt_no, std::vector<int>{var_index});
}

int ModifiesRelationshipStore::GetVarIndex(int stmt_no) const {
    auto var_indices = stmt_var_.GetVals(stmt_no);
    return var_indices.empty() ? 0 : var_indices[0];
}

void ModifiesRelationshipStore::Compile(
        const TypeStatementsStore& type_statement_store,
        const ContainerForest& forest, const StmtlstParentStore& stmtlst_parent,
        const StmtlstStatementsStore& stmtlst_stmt) {
    FillVars();

    std::array<StmtType, 2> direct_stmt_types{
            {StmtType::kAssign, StmtType::kRead}};
    for (const auto& stmt_type : direct_stmt_types) {
        auto& stmt_var_pairs =
                stmt_var_pairs_.at(static_cast<int>(stmt_type) - 1);
        AddDirectRel(stmt_var_pairs,
                     type_statement_store.GetStatements(stmt_type));
    }

    AddContainerRel(forest, stmtlst_parent, stmtlst_stmt, type_statement_store);
}

// Add indirect Modifies for container statements.
void ModifiesRelationshipStore::AddContainerRel(
        const ContainerForest& forest, const StmtlstParentStore& stmtlst_parent,
        const StmtlstStatementsStore& stmtlst_stmt,
        const TypeStatementsStore& type_statement_store) {
    BitVec2D if_added(num_stmts + 1, num_vars + 1);
    BitVec2D while_added(num_stmts + 1, num_vars + 1);

    std::array<StmtType, 4> indirect_stmt_types{{StmtType::kAssign,
                                                 StmtType::kRead, StmtType::kIf,
                                                 StmtType::kWhile}};
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
