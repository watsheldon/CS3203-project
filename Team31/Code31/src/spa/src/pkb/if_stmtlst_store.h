#ifndef SPA_SRC_SPA_SRC_PKB_IF_STMTLST_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_IF_STMTLST_STORE_H_

#include <unordered_map>

namespace spa {
class IfStmtlstStore {
  public:
    IfStmtlstStore();
    void Set(int stmt_no, int then_index, int else_index);
    int GetStmtNo(int stmtlst_index);
    std::pair<int, int> GetStmtlstPair(int stmt_no);

  private:
    std::unordered_map<int, std::pair<int, char>> if_to_stmtlst;
    std::unordered_map<int, int> stmtlst_to_if;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_PKB_IF_STMTLST_STORE_H_
