#ifndef SPA_SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_

#include <vector>

#include "pkb/knowledge_base.h"

namespace spa {
class UsesRelationshipStore {
  public:
    UsesRelationshipStore(size_t stmtSize, size_t varSize);
    void Set(int stmt_no, std::vector<int> &&var_indices);
    [[nodiscard]] std::vector<int> GetStmtNo(int var_index) const; //Gets a list of stmt that uses var_index
    [[nodiscard]] std::vector<int> GetVarIndex(int stmt_no) const;  // Gets a list of var_index that is used in given stmt_no
 
  private:
    std::vector<std::vector<int>> stmt_to_varlst_; // stmtNo to list of var_index
    std::vector<std::vector<int>> var_to_stmtlst_; // var_index to a list of stmt used by var_index
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_PKB_STORE_USES_RELATIONSHIP_STORE_H_
