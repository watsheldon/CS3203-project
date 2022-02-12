#ifndef SPA_SRC_SPA_SRC_PKB_WHILE_STMTLST_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_WHILE_STMTLST_STORE_H_

#include "knowledge_base.h"

namespace spa {
class WhileStmtlstStore {
  public:
    WhileStmtlstStore(size_t stmt, size_t stmtlst);
    void Set(Index<kStmt> stmt_no, Index<kStmtLst> stmtlst_index);
    [[nodiscard]] Index<kStmt> GetStmtNo(Index<kStmtLst> stmtlst_index) const;
    [[nodiscard]] Index<kStmtLst> GetStmtlst(Index<kStmt> stmt_no) const;

  private:
    std::vector<int> while_to_stmtlst;
    std::vector<int> stmtlst_to_while;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_PKB_WHILE_STMTLST_STORE_H_
