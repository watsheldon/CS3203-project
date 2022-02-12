#ifndef SPA_SRC_SPA_SRC_PKB_PROCEDURE_STMTLST_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_PROCEDURE_STMTLST_STORE_H_

#include <unordered_map>
#include <vector>

#include "knowledge_base.h"

namespace spa {
class ProcedureStmtlstStore {
  public:
    explicit ProcedureStmtlstStore(size_t proc, size_t stmtlst);
    void Set(Index<kProc> proc_index, Index<kStmtLst> stmtlst_index);
    [[nodiscard]] Index<kProc> GetProcIndex(
            Index<kStmtLst> stmtlst_index) const;
    [[nodiscard]] Index<kStmtLst> GetStmtlst(Index<kProc> proc_index) const;

  private:
    std::vector<int> proc_to_stmtlst;
    std::vector<int> stmtlst_to_proc;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_PKB_PROCEDURE_STMTLST_STORE_H_
