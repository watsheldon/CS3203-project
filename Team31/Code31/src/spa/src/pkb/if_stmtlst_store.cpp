#include "if_stmtlst_store.h"

using namespace spa;

spa::IfStmtlstStore::IfStmtlstStore() {}

void spa::IfStmtlstStore::Set(int stmt_no, int then_index, int else_index) {
    if_to_stmtlst[stmt_no] = {then_index, else_index};
    stmtlst_to_if[then_index] = stmt_no;
    stmtlst_to_if[else_index] = stmt_no;
}

int spa::IfStmtlstStore::GetStmtNo(int stmtlst_index) {
    if (stmtlst_to_if.find(stmtlst_index) == stmtlst_to_if.end()) {
        return -1;
    } else {
        return stmtlst_to_if.at(stmtlst_index);
    }
}

std::pair<int, int> spa::IfStmtlstStore::GetStmtlstPair(int stmt_no) {
    if (if_to_stmtlst.find(stmt_no) == if_to_stmtlst.end()) {
        return {-1, -1};
    } else {
        return if_to_stmtlst.at(stmt_no);
    }
}
