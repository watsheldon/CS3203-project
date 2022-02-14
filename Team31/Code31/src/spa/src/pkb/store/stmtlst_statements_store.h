#ifndef SPA_SRC_SPA_SRC_PKB_STMTLST_STATEMENTS_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_STMTLST_STATEMENTS_STORE_H_

#include <vector>

#include "pkb/knowledge_base.h"

namespace spa {
struct StmtlstIndexPos {
    int stmtlst_index = 0;
    int pos_in_stmtlst = 0;
};
class StmtlstStatementsStore {
  public:
    StmtlstStatementsStore(size_t stmtlst, size_t stmt);
    void Set(int stmtlst_index, std::vector<int> &&stmtlst);
    [[nodiscard]] int GetIndex(int stmt_no) const;
    [[nodiscard]] int GetPos(int stmt_no) const;
    [[nodiscard]] StmtlstIndexPos GetIndexPos(int stmt_no) const;
    [[nodiscard]] std::vector<int> GetStatements(int stmtlst_index);

  private:
    std::vector<std::vector<int>> stmtlst_to_statements_;
    std::vector<StmtlstIndexPos> statement_to_stmtlst_;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_PKB_STMTLST_STATEMENTS_STORE_H_
