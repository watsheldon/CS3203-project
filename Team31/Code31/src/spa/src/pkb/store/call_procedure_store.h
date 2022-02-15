#ifndef SPA_SRC_SPA_SRC_PKB_STORE_CALL_PROCEDURE_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_STORE_CALL_PROCEDURE_STORE_H_

#include "pkb/knowledge_base.h"

namespace spa {
class CallProcedureStore {
  public:
    explicit CallProcedureStore(size_t stmt_count, size_t proc_count);
    void Set(int call_stmt, int proc_index);
    int GetProcIndex(int call_stmt);
    std::vector<int> GetCalls(int proc_index);

  private:
    std::vector<int> call_to_proc_;
    std::vector<std::vector<int>> proc_to_calls_;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_PKB_STORE_CALL_PROCEDURE_STORE_H_
