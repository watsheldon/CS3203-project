#ifndef SPA_SRC_SPA_SRC_PKB_STORE_PROCEDURE_STMTLST_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_STORE_PROCEDURE_STMTLST_STORE_H_

#include <unordered_map>
#include <vector>

#include "pkb/knowledge_base.h"

namespace spa {
class ProcedureStmtlstStore {
  public:
    explicit ProcedureStmtlstStore(size_t proc, size_t stmtlst);
    void Set(int proc_index, int stmtlst_index);
    [[nodiscard]] int GetProcIndex(int stmtlst_index) const;
    [[nodiscard]] int GetStmtlst(int proc_index) const;

  private:
    std::vector<int> proc_to_stmtlst_;
    std::vector<int> stmtlst_to_proc_;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_PKB_STORE_PROCEDURE_STMTLST_STORE_H_
