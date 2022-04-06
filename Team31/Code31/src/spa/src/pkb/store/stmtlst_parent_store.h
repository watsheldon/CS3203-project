#ifndef SRC_SPA_SRC_PKB_STORE_STMTLST_PARENT_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_STMTLST_PARENT_STORE_H_

#include <vector>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "common/index.h"

namespace spa {
class StmtlstParentStore {
  public:
    struct IfPairs {
        int then_index = 0;
        int else_index = 0;
    };
    enum ParentType { kNone, kProc, kIf, kWhile };
    struct StmtLstParent {
        ParentType type = kNone;
        int index = 0;
    };

    StmtlstParentStore(size_t proc_count, size_t stmt_count,
                       size_t stmtlst_count);
    void Set(Index<SetEntityType::kProc> proc_no,
             Index<SetEntityType::kStmtLst> index);
    void Set(Index<SetEntityType::kStmt> while_no,
             Index<SetEntityType::kStmtLst> index);
    void Set(Index<SetEntityType::kStmt> if_no,
             Index<SetEntityType::kStmtLst> then_index,
             Index<SetEntityType::kStmtLst> else_index);
    [[nodiscard]] const StmtLstParent &GetParent(
            StmtLstIndex stmtlst_index) const;
    [[nodiscard]] StmtLstIndex GetProcStmtLst(ProcIndex proc_index) const;
    [[nodiscard]] StmtLstIndex GetWhileStmtLst(StmtNo stmt_no) const;
    [[nodiscard]] IfPairs GetIfStmtLst(StmtNo stmt_no) const;

  private:
    std::vector<StmtLstIndex> proc_stmtlsts_;
    std::vector<StmtLstIndex> stmt_stmtlsts_;
    std::vector<StmtLstIndex> else_stmtlsts_;
    std::vector<StmtLstParent> stmtlst_parent_;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_PKB_STORE_STMTLST_PARENT_STORE_H_
