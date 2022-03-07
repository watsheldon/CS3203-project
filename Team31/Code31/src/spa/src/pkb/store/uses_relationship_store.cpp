#include "uses_relationship_store.h"

namespace spa {
UsesRelationshipStore::UsesRelationshipStore(size_t stmtSize, size_t varSize)
        : IndexBimap<std::vector<int>>(stmtSize, varSize) {}

void UsesRelationshipStore::Set(int stmt_no, std::vector<int>&& var_indices) {
    IndexBimap<std::vector<int>>::Set(
            stmt_no, std::forward<std::vector<int>>(var_indices));
}

const std::vector<int>& UsesRelationshipStore::GetVarIndex(int stmt_no) const {
    return IndexBimap<std::vector<int>>::GetVals(stmt_no);
}

const std::vector<int>& UsesRelationshipStore::GetStmtNo(int var_index) const {
    return IndexBimap<std::vector<int>>::GetKeys(var_index);
}
}  // namespace spa
