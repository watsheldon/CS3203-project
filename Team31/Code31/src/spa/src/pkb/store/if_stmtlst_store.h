#ifndef SPA_SRC_SPA_SRC_PKB_STORE_IF_STMTLST_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_STORE_IF_STMTLST_STORE_H_

#include "pkb/knowledge_base.h"

namespace spa {
struct IfPairs {
    int then_index = 0;
    int else_index = 0;
};
class IfStmtlstStore {
  public:
    IfStmtlstStore(size_t stmt, size_t stmtlst);
    void Set(int stmt_no, int then_index, int else_index);
    [[nodiscard]] int GetStmtNo(int stmtlst_index) const;
    [[nodiscard]] int GetThenStmtlst(int stmt_no) const;
    [[nodiscard]] int GetElseStmtlst(int stmt_no) const;
    [[nodiscard]] IfPairs GetBothStmtlst(int stmt_no) const;

  private:
    std::vector<IfPairs> if_to_stmtlst_;
    std::vector<int> stmtlst_to_if_;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_PKB_STORE_IF_STMTLST_STORE_H_
