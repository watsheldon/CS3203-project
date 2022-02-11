#include "procedure_stmtlst_store.h"

using namespace spa;

spa::ProcedureStmtlstStore::ProcedureStmtlstStore(size_t size)
        : proc_to_stmtlst(size) {}

void spa::ProcedureStmtlstStore::Set(int stmt_no, int stmtlst_index) {
    proc_to_stmtlst[stmt_no] = stmtlst_index;
    stmtlst_to_proc[stmtlst_index] = stmt_no;
}

int spa::ProcedureStmtlstStore::GetProcIndex(int stmtlst_index) {
    if (stmtlst_to_proc.find(stmtlst_index) == stmtlst_to_proc.end()) {
        return -1;
    } else {
        return stmtlst_to_proc.at(stmtlst_index);
    }
}

int spa::ProcedureStmtlstStore::GetStmtlst(int proc_index) {
    if (proc_index >= proc_to_stmtlst.size()) {
        return -1;
    } else {
        return proc_to_stmtlst.at(proc_index);
    }
}
