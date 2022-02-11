#ifndef SPA_SRC_SPA_SRC_PKB_WHILE_STMTLST_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_WHILE_STMTLST_STORE_H_

#include <unordered_map>

namespace spa {
class WhileStmtlstStore {
  public:
    WhileStmtlstStore();
    void Set(int stmt_no, int stmtlst_index);
    int GetStmtNo(int stmtlst_index);
    int GetStmtlst(int stmt_no);

  private:
    std::unordered_map<int, int> while_to_stmtlst;
    std::unordered_map<int, int> stmtlst_to_while;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_PKB_WHILE_STMTLST_STORE_H_
