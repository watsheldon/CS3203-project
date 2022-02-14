#include "stmtlst_statements_store.h"

#include "pkb/knowledge_base.h"

namespace spa {
spa::StmtlstStatementsStore::StmtlstStatementsStore(size_t stmtlst_count,
                                                    size_t stmt_count)
        : stmtlst_to_statements_(stmtlst_count + 1),
          statement_to_stmtlst_(stmt_count + 1, 0) {}
void spa::StmtlstStatementsStore::Set(int stmtlst_index,
                                      std::vector<int> &&stmtlst) {
    stmtlst_to_statements_[stmtlst_index] = stmtlst;
    for (auto &i : stmtlst) {
        statement_to_stmtlst_[i] = stmtlst_index;
    }
}

int spa::StmtlstStatementsStore::GetStmtlst(int stmt_no) const {
    return statement_to_stmtlst_[stmt_no];
}
std::vector<int> spa::StmtlstStatementsStore::GetStatements(int stmtlst_index) {
    return stmtlst_to_statements_[stmtlst_index];
}
}  // namespace spa
