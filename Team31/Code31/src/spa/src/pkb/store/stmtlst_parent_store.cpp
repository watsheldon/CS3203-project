#include "stmtlst_parent_store.h"

#include <cstddef>

#include "common/entity_type_enum.h"
#include "common/index.h"

namespace spa {
StmtlstParentStore::StmtlstParentStore(size_t proc_count, size_t stmt_count,
                                       size_t stmtlst_count)
        : proc_stmtlsts_(proc_count + 1),
          stmt_stmtlsts_(stmt_count + 1),
          else_stmtlsts_(stmt_count + 1),
          stmtlst_parent_(stmtlst_count + 1) {}
void StmtlstParentStore::Set(Index<SetEntityType::kProc> proc_no,
                             Index<SetEntityType::kStmtLst> index) {
    proc_stmtlsts_[proc_no.value] = index.value;
    stmtlst_parent_[index.value] = {kProc, proc_no.value};
}
void StmtlstParentStore::Set(Index<SetEntityType::kStmt> while_no,
                             Index<SetEntityType::kStmtLst> index) {
    stmt_stmtlsts_[while_no.value] = index.value;
    stmtlst_parent_[index.value] = {kWhile, while_no.value};
}
void StmtlstParentStore::Set(Index<SetEntityType::kStmt> if_no,
                             Index<SetEntityType::kStmtLst> then_index,
                             Index<SetEntityType::kStmtLst> else_index) {
    stmt_stmtlsts_[if_no.value] = then_index.value;
    else_stmtlsts_[if_no.value] = else_index.value;
    stmtlst_parent_[then_index.value] =
            stmtlst_parent_[else_index.value] = {kIf, if_no.value};
}
const StmtlstParentStore::StmtLstParent& StmtlstParentStore::GetParent(
        int stmtlst_index) const {
    return stmtlst_parent_[stmtlst_index];
}
int StmtlstParentStore::GetProcStmtLst(int proc_index) const {
    return proc_stmtlsts_[proc_index];
}
int StmtlstParentStore::GetWhileStmtLst(int stmt_no) const {
    return stmt_stmtlsts_[stmt_no];
}
StmtlstParentStore::IfPairs StmtlstParentStore::GetIfStmtLst(
        int stmt_no) const {
    return {stmt_stmtlsts_[stmt_no], else_stmtlsts_[stmt_no]};
}
}  // namespace spa
