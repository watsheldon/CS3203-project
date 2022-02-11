#ifndef SPA_SRC_SPA_SRC_PKB_PROCEDURE_STMTLST_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_PROCEDURE_STMTLST_STORE_H_

#include <unordered_map>
#include <vector>

namespace spa {
class ProcedureStmtlstStore {
  public:
    explicit ProcedureStmtlstStore(size_t size);
    void Set(int proc_index, int stmtlst_index);
    int GetProcIndex(int stmtlst_index);
    int GetStmtlst(int proc_index);

  private:
    std::vector<int> proc_to_stmtlst;
    std::unordered_map<int, int> stmtlst_to_proc;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_PKB_PROCEDURE_STMTLST_STORE_H_
