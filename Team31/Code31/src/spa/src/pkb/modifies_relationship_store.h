#ifndef SPA_SRC_SPA_SRC_PKB_MODIFIES_RELATIONSHIP_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_MODIFIES_RELATIONSHIP_STORE_H_

#include <vector>

#include "knowledge_base.h"

namespace spa {
class ModifiesRelationshipStore {
  public:
    ModifiesRelationshipStore(size_t stmtSize, size_t varSize);
    void Set(int stmt_no, int var_index); // Set modify relationship between stmt_no and var_index
    [[nodiscard]] std::vector<int> GetStmtNo(int var_index) const; //Gets a list of stmt that modifies var_index
    [[nodiscard]] int GetVarIndex(int stmt_no) const; //Gets a var_index that is modified in given stmt_no

  private:
    std::vector<int> stmt_to_var_;  // stmtNo to var_index
    std::vector<std::vector<int>> var_to_stmtlst_;  // var_index to a list of stmt that modifies by var_index
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_PKB_MODIFIES_RELATIONSHIP_STORE_H_
