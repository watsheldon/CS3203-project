#ifndef SPA_SRC_SPA_SRC_PKB_IF_STMTLST_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_IF_STMTLST_STORE_H_

#include "knowledge_base.h"

namespace spa {
struct IfPairs {
    int then_index = 0;
    int else_index = 0;
};
struct IfIndices {
    Index<kStmtLst> then_index = Index<kStmtLst>(0);
    Index<kStmtLst> else_index = Index<kStmtLst>(0);
};
class IfStmtlstStore {
  public:
    IfStmtlstStore(size_t stmt, size_t stmtlst);
    void Set(Index<kStmt> stmt_no, Index<kStmtLst> then_index,
             Index<kStmtLst> else_index);
    [[nodiscard]] Index<kStmt> GetStmtNo(Index<kStmtLst> stmtlst_index) const;
    [[nodiscard]] Index<kStmtLst> GetThenStmtlst(Index<kStmt> stmt_no) const;
    [[nodiscard]] Index<kStmtLst> GetElseStmtlst(Index<kStmt> stmt_no) const;
    [[nodiscard]] IfIndices GetBothStmtlst(Index<kStmt> stmt_no) const;

  private:
    std::vector<IfPairs> if_to_stmtlst;
    std::vector<int> stmtlst_to_if;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_PKB_IF_STMTLST_STORE_H_
