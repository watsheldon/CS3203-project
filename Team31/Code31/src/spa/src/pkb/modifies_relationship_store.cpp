#include "modifies_relationship_store.h"

namespace spa {
spa::ModifiesRelationshipStore::ModifiesRelationshipStore(size_t stmtSize,
                                                          size_t varSize)
        : stmt_to_var_(stmtSize + 1, 0), var_to_stmtlst_(varSize + 1) {}

void spa::ModifiesRelationshipStore::Set(int stmt_no, 
                                         int var_index) {
    stmt_to_var_[stmt_no] = var_index;
    var_to_stmtlst_[var_index].emplace_back(stmt_no);
}

std::vector<int> spa::ModifiesRelationshipStore::GetStmtNo(int var_index) const {
    return var_to_stmtlst_[var_index];
}

int spa::ModifiesRelationshipStore::GetVarIndex(int stmt_no) const {
    return stmt_to_var_[stmt_no];
}
}  // namespace spa
