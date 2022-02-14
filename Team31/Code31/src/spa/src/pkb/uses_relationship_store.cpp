#include "uses_relationship_store.h"

namespace spa {
spa::UsesRelationshipStore::UsesRelationshipStore(size_t stmtSize,
                                                  size_t varSize)
        : stmt_to_varlst_(stmtSize + 1), var_to_stmtlst_(varSize + 1) {}

void spa::UsesRelationshipStore::Set(int stmt_no, 
                                     std::vector<int> &&var_indices) {
    stmt_to_varlst_[stmt_no] = var_indices;
    for (auto &i : var_indices) {
        var_to_stmtlst_[i].emplace_back(stmt_no);
    }
}

std::vector<int> spa::UsesRelationshipStore::GetStmtNo(int var_index) const {
    return var_to_stmtlst_[var_index];
}

std::vector<int> spa::UsesRelationshipStore::GetVarIndex(int stmt_no) const {
    return stmt_to_varlst_[stmt_no];
}
}  // namespace spa
