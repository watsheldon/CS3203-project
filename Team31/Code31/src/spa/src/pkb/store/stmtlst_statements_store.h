#ifndef SPA_SRC_SPA_SRC_PKB_STORE_STMTLST_STATEMENTS_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_STORE_STMTLST_STATEMENTS_STORE_H_

#include <vector>

#include "pkb/knowledge_base.h"

namespace spa {
class StmtlstStatementsStore {
  public:
    StmtlstStatementsStore(size_t stmtlst, size_t stmt);
    void Set(int stmtlst_index, std::vector<int> &&stmtlst);
    [[nodiscard]] int GetStmtlst(int stmt_no) const;
    [[nodiscard]] std::vector<int> GetStatements(int stmtlst_index);

  private:
    std::vector<std::vector<int>> stmtlst_to_statements_;
    std::vector<int> statement_to_stmtlst_;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_PKB_STORE_STMTLST_STATEMENTS_STORE_H_
