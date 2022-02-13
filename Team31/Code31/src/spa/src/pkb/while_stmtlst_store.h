#ifndef SPA_SRC_SPA_SRC_PKB_WHILE_STMTLST_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_WHILE_STMTLST_STORE_H_

#include "knowledge_base.h"

namespace spa {
class WhileStmtlstStore {
  public:
    WhileStmtlstStore(size_t stmt, size_t stmtlst);
    void Set(int stmt_no, int stmtlst_index);
    [[nodiscard]] int GetStmtNo(int stmtlst_index) const;
    [[nodiscard]] int GetStmtlst(int stmt_no) const;

  private:
    std::vector<int> while_to_stmtlst_;
    std::vector<int> stmtlst_to_while_;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_PKB_WHILE_STMTLST_STORE_H_
