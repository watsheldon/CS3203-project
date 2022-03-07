#include "modifies_relationship_store.h"

namespace spa {
spa::ModifiesRelationshipStore::ModifiesRelationshipStore(size_t stmtSize,
                                                          size_t varSize)
        : stmt_var_(stmtSize, varSize) {}

void spa::ModifiesRelationshipStore::Set(int stmt_no, int var_index) {
    stmt_var_.Set(stmt_no, std::vector<int>{var_index});
}

const std::vector<int>& spa::ModifiesRelationshipStore::GetStmtNo(
        int var_index) const {
    return stmt_var_.GetKeys(var_index);
}

int spa::ModifiesRelationshipStore::GetVarIndex(int stmt_no) const {
    return stmt_var_.GetVals(stmt_no)[0];
}
}  // namespace spa
