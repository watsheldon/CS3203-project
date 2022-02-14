#include "if_stmtlst_store.h"

#include "knowledge_base.h"

namespace spa {
spa::IfStmtlstStore::IfStmtlstStore(size_t stmt_count, size_t stmtlst_count)
        : if_to_stmtlst_(stmt_count + 1), stmtlst_to_if_(stmtlst_count + 1) {}

void spa::IfStmtlstStore::Set(int stmt_no, int then_index, int else_index) {
    if_to_stmtlst_[stmt_no] = {then_index, else_index};
    stmtlst_to_if_[then_index] = stmt_no;
    stmtlst_to_if_[else_index] = stmt_no;
}

int spa::IfStmtlstStore::GetStmtNo(int stmtlst_index) const {
    return stmtlst_to_if_[stmtlst_index];
}

int spa::IfStmtlstStore::GetThenStmtlst(int stmt_no) const {
    return if_to_stmtlst_[stmt_no].then_index;
}
int spa::IfStmtlstStore::GetElseStmtlst(int stmt_no) const {
    return if_to_stmtlst_[stmt_no].else_index;
}
IfPairs IfStmtlstStore::GetBothStmtlst(int stmt_no) const {
    return if_to_stmtlst_[stmt_no];
}
}  // namespace spa
