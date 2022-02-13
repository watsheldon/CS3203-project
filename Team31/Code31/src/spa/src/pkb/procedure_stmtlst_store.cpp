#include "procedure_stmtlst_store.h"

#include "knowledge_base.h"

namespace spa {
spa::ProcedureStmtlstStore::ProcedureStmtlstStore(size_t proc_count,
                                                  size_t stmtlst_count)
        : proc_to_stmtlst_(proc_count), stmtlst_to_proc_(stmtlst_count + 1) {}

void spa::ProcedureStmtlstStore::Set(int stmt_no, int stmtlst_index) {
    proc_to_stmtlst_[stmt_no] = stmtlst_index;
    stmtlst_to_proc_[stmtlst_index] = stmt_no;
}

int spa::ProcedureStmtlstStore::GetProcIndex(int stmtlst_index) const {
    return stmtlst_to_proc_.at(stmtlst_index);
}

int spa::ProcedureStmtlstStore::GetStmtlst(int proc_index) const {
    return proc_to_stmtlst_.at(proc_index);
}
}  // namespace spa
