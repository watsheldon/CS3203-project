#include "stmtlst_statements_store.h"

#include "pkb/knowledge_base.h"

namespace spa {
spa::StmtlstStatementsStore::StmtlstStatementsStore(size_t stmtlst_count,
                                                    size_t stmt_count)
        : stmtlst_to_statements_(stmtlst_count + 1),
          statement_to_stmtlst_(stmt_count + 1) {}
void spa::StmtlstStatementsStore::Set(int stmtlst_index,
                                      std::vector<int> &&stmtlst) {
    stmtlst_to_statements_[stmtlst_index] = stmtlst;
    int pos = 0;
    for (auto &i : stmtlst) {
        statement_to_stmtlst_[i] = {stmtlst_index, pos};
        pos++;
    }
}

int spa::StmtlstStatementsStore::GetIndex(int stmt_no) const {
    return statement_to_stmtlst_[stmt_no].stmtlst_index;
}
int spa::StmtlstStatementsStore::GetPos(int stmt_no) const {
    return statement_to_stmtlst_[stmt_no].pos_in_stmtlst;
}
StmtlstIndexPos spa::StmtlstStatementsStore::GetIndexPos(int stmt_no) const {
    return statement_to_stmtlst_[stmt_no];
}
std::vector<int> spa::StmtlstStatementsStore::GetStatements(int stmtlst_index) {
    return stmtlst_to_statements_[stmtlst_index];
}
}  // namespace spa
