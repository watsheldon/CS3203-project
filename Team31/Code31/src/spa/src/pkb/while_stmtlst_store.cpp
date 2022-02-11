#include "while_stmtlst_store.h"

using namespace spa;

spa::WhileStmtlstStore::WhileStmtlstStore() {}

void spa::WhileStmtlstStore::Set(int stmt_no, int stmtlst_index) {
    while_to_stmtlst[stmt_no] = stmtlst_index;
    stmtlst_to_while[stmtlst_index] = stmt_no;
}

int spa::WhileStmtlstStore::GetStmtNo(int stmtlst_index) {
    if (stmtlst_to_while.find(stmtlst_index) == stmtlst_to_while.end()) {
        return -1;
    } else {
        return stmtlst_to_while.at(stmtlst_index);
    }
}

int spa::WhileStmtlstStore::GetStmtlst(int stmt_no) {
    if (while_to_stmtlst.find(stmt_no) == while_to_stmtlst.end()) {
        return -1;
    } else {
        return while_to_stmtlst.at(stmt_no);
    }
}
