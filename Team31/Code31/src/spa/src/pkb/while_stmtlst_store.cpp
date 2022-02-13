#include "while_stmtlst_store.h"
namespace spa {
spa::WhileStmtlstStore::WhileStmtlstStore(size_t stmt_count,
                                          size_t stmtlst_count)
        : while_to_stmtlst_(stmt_count + 1),
          stmtlst_to_while_(stmtlst_count + 1) {}

void spa::WhileStmtlstStore::Set(int stmt_no, int stmtlst_index) {
    while_to_stmtlst_[stmt_no] = stmtlst_index;
    stmtlst_to_while_[stmtlst_index] = stmt_no;
}

int spa::WhileStmtlstStore::GetStmtNo(int stmtlst_index) const {
    return stmtlst_to_while_.at(stmtlst_index);
}

int spa::WhileStmtlstStore::GetStmtlst(int stmt_no) const {
    return while_to_stmtlst_.at(stmt_no);
}
}  // namespace spa
