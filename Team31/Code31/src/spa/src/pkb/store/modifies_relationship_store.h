#ifndef SRC_SPA_SRC_PKB_STORE_MODIFIES_RELATIONSHIP_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_MODIFIES_RELATIONSHIP_STORE_H_

#include <vector>

#include "common/entity_type_enum.h"

namespace spa {
class ModifiesRelationshipStore {
  public:
    ModifiesRelationshipStore(size_t stmtSize, size_t varSize);
    void Set(int stmt_no, int var_index);
    [[nodiscard]] const std::vector<int>& GetStmtNo(int var_index) const;
    [[nodiscard]] int GetVarIndex(int stmt_no) const;

  private:
    std::vector<int> stmt_to_var_;
    std::vector<std::vector<int>> var_to_stmtlst_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_MODIFIES_RELATIONSHIP_STORE_H_
