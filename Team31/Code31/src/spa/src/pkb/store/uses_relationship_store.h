#ifndef SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_

#include <vector>

#include "pkb/knowledge_base.h"

namespace spa {
class UsesRelationshipStore {
  public:
    UsesRelationshipStore(size_t stmtSize, size_t varSize);
    void Set(int stmt_no, std::vector<int> &&var_indices);
    [[nodiscard]] std::vector<int> GetStmtNo(int var_index) const;
    [[nodiscard]] std::vector<int> GetVarIndex(int stmt_no) const;

  private:
    std::vector<std::vector<int>> stmt_to_varlst_;
    std::vector<std::vector<int>> var_to_stmtlst_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_
