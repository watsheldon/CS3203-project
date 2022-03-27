#include "modifies_relationship_store.h"

#include <vector>

#include "type_statements_store.h"

namespace spa {
void ModifiesRelationshipStore::Set(int stmt_no, int var_index) {
    stmt_var_.Set(stmt_no, std::vector<int>{var_index});
}
void ModifiesRelationshipStore::AddConditionRel(
        const AuxiliaryData& data_store) {}
void ModifiesRelationshipStore::AddAllIndirectRel(
        const AuxiliaryData& data_store) {
    FillIndirectRels(indirect_stmt_types_, data_store);
}
void ModifiesRelationshipStore::AddAllDirectRel(
        const TypeStatementsStore& store) {
    FillDirectRels(direct_stmt_types_, store);
}
}  // namespace spa
