#ifndef SRC_SPA_SRC_PKB_STORE_MODIFIES_RELATIONSHIP_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_MODIFIES_RELATIONSHIP_STORE_H_

#include "index_bimap.h"

namespace spa {
class ModifiesRelationshipStore {
  public:
    ModifiesRelationshipStore(size_t stmtSize, size_t varSize);
    void Set(int stmt_no, int var_index);
    [[nodiscard]] const std::vector<int>& GetStmtNo(int var_index) const;
    [[nodiscard]] int GetVarIndex(int stmt_no) const;

  private:
    IndexBimap<std::vector<int>> stmt_var_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_MODIFIES_RELATIONSHIP_STORE_H_
