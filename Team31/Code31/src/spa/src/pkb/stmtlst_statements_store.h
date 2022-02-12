#ifndef SPA_SRC_SPA_SRC_PKB_STMTLST_STATEMENTS_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_STMTLST_STATEMENTS_STORE_H_

#include <vector>

#include "knowledge_base.h"

namespace spa {
class StmtlstStatementsStore {
  public:
    StmtlstStatementsStore(size_t stmtlst, size_t stmt);
    void Set(Index<kStmtLst> stmtlst_index,
             std::vector<Index<kStmt>> &&stmtlst);
    [[nodiscard]] Index<kStmtLst> GetStmtlst(Index<kStmt> stmt_no) const;
    [[nodiscard]] std::vector<Index<kStmt>> GetStatements(
            Index<kStmtLst> stmtlst_index);

  private:
    std::vector<std::vector<int>> stmtlst_to_statements;
    std::vector<int> statement_to_stmtlst;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_PKB_STMTLST_STATEMENTS_STORE_H_
