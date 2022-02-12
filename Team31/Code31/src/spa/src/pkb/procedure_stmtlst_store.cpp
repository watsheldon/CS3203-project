#include "procedure_stmtlst_store.h"

#include "knowledge_base.h"

namespace spa {
spa::ProcedureStmtlstStore::ProcedureStmtlstStore(size_t proc, size_t stmtlst)
        : proc_to_stmtlst(proc, 0), stmtlst_to_proc(stmtlst, 0) {}

void spa::ProcedureStmtlstStore::Set(Index<kProc> stmt_no,
                                     Index<kStmtLst> stmtlst_index) {
    proc_to_stmtlst[stmt_no.value] = stmtlst_index.value;
    stmtlst_to_proc[stmtlst_index.value] = stmt_no.value;
}

Index<kProc> spa::ProcedureStmtlstStore::GetProcIndex(
        Index<kStmtLst> stmtlst_index) const {
    return Index<kProc>(stmtlst_to_proc.at(stmtlst_index.value));
}

Index<kStmtLst> spa::ProcedureStmtlstStore::GetStmtlst(
        Index<kProc> proc_index) const {
    return Index<kStmtLst>(proc_to_stmtlst.at(proc_index.value));
}
}  // namespace spa