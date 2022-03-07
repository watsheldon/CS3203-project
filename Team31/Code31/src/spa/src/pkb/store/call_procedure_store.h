#ifndef SRC_SPA_SRC_PKB_STORE_CALL_PROCEDURE_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_CALL_PROCEDURE_STORE_H_

#include <vector>

namespace spa {
class CallProcedureStore {
  public:
    explicit CallProcedureStore(size_t stmt_count, size_t proc_count);
    void Set(int call_stmt, int proc_index);
    [[nodiscard]] int GetProcIndex(int call_stmt) const;
    [[nodiscard]] std::vector<int> GetCalls(int proc_index) const;

  private:
    std::vector<int> call_to_proc_;
    std::vector<std::vector<int>> proc_to_calls_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_CALL_PROCEDURE_STORE_H_
