#include "modifies_relationship_store.h"

#include <cassert>
#include <vector>

#include "common/bitvec2d.h"
#include "pkb/secondary_structure/container_forest.h"
#include "stmtlst_parent_store.h"
#include "stmtlst_statements_store.h"
#include "type_statements_store.h"

namespace spa {
ModifiesRelationshipStore::ModifiesRelationshipStore(size_t stmt_size,
                                                     size_t var_size)
        : UsesModifies(stmt_size, var_size) {}

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
    UsesModifies::AggregateVars();
    UsesModifies::CompileBasic(
            assign_var_pairs_,
            type_statement_store.GetStatements(StmtType::kAssign));
    UsesModifies::CompileBasic(
            read_var_pairs_,
            type_statement_store.GetStatements(StmtType::kRead));
    CompileContainers(forest, stmtlst_parent, stmtlst_stmt,
                      type_statement_store);
}

// Add indirect Modifies for container statements.
void ModifiesRelationshipStore::CompileContainers(
        const ContainerForest& forest, const StmtlstParentStore& stmtlst_parent,
        const StmtlstStatementsStore& stmtlst_stmt,
        const TypeStatementsStore& type_statement_store) {
    BitVec2D if_added(num_stmts + 1, num_vars + 1);
    BitVec2D while_added(num_stmts + 1, num_vars + 1);
    UsesModifies::AddAncestorsOnly(assign_var_pairs_, stmtlst_stmt,
                                   stmtlst_parent, forest, if_added,
                                   while_added);
    UsesModifies::AddAncestorsOnly(read_var_pairs_, stmtlst_stmt,
                                   stmtlst_parent, forest, if_added,
                                   while_added);
    UsesModifies::AddAncestorsOnly(if_var_pairs_, stmtlst_stmt, stmtlst_parent,
                                   forest, if_added, while_added);
    UsesModifies::AddAncestorsOnly(while_var_pairs_, stmtlst_stmt,
                                   stmtlst_parent, forest, if_added,
                                   while_added);
    UsesModifies::AggregateStmts();
    UsesModifies::UpdateStmtVar();
}
}  // namespace spa
