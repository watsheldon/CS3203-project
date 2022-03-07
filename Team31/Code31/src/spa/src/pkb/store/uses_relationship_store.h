#ifndef SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_

#include "index_bimap.h"

namespace spa {
class UsesRelationshipStore : public IndexBimap<std::vector<int>> {
  public:
    UsesRelationshipStore(size_t stmtSize, size_t varSize);
    void Set(int stmt_no, std::vector<int> &&var_indices);
    [[nodiscard]] const std::vector<int> &GetVarIndex(int stmt_no) const;
    [[nodiscard]] const std::vector<int> &GetStmtNo(int var_index) const;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_
