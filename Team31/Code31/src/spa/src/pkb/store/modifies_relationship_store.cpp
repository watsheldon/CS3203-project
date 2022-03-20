#include "modifies_relationship_store.h"

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

void ModifiesRelationshipStore::AddConditionRel(
        const ContainerForest& forest, const StmtlstParentStore& stmtlst_parent,
        const StmtlstStatementsStore& stmtlst_stmt,
        const TypeStatementsStore& type_statement_store, PairBitmap& bitmaps) {
    return;
}

void ModifiesRelationshipStore::AddAllIndirectRel(
        const TypeStatementsStore& type_statement_store,
        const StmtlstStatementsStore& stmtlst_stmt,
        const StmtlstParentStore& stmtlst_parent, const ContainerForest& forest,
        PairBitmap& bitmaps) {
    FillIndirectRels(indirect_stmt_types_, type_statement_store, stmtlst_stmt,
                     stmtlst_parent, forest, bitmaps);
}

void ModifiesRelationshipStore::AddAllDirectRel(
        const TypeStatementsStore& store) {
    FillDirectRels(direct_stmt_types_, store);
}
}  // namespace spa
