#include "while_stmtlst_store.h"
namespace spa {
spa::WhileStmtlstStore::WhileStmtlstStore(size_t stmt, size_t stmtlst)
        : while_to_stmtlst(stmt, 0), stmtlst_to_while(stmtlst, 0) {}

void spa::WhileStmtlstStore::Set(Index<kStmt> stmt_no,
                                 Index<kStmtLst> stmtlst_index) {
    while_to_stmtlst[stmt_no.value] = stmtlst_index.value;
    stmtlst_to_while[stmtlst_index.value] = stmt_no.value;
}

Index<kStmt> spa::WhileStmtlstStore::GetStmtNo(
        Index<kStmtLst> stmtlst_index) const {
    return Index<kStmt>(stmtlst_to_while.at(stmtlst_index.value));
}

Index<kStmtLst> spa::WhileStmtlstStore::GetStmtlst(Index<kStmt> stmt_no) const {
    return Index<kStmtLst>(while_to_stmtlst.at(stmt_no.value));
}
}  // namespace spa