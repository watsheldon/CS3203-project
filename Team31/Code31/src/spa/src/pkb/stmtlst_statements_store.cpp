#include "stmtlst_statements_store.h"

#include "knowledge_base.h"

namespace spa {
spa::StmtlstStatementsStore::StmtlstStatementsStore(size_t stmtlst, size_t stmt)
        : stmtlst_to_statements(stmtlst), statement_to_stmtlst(stmt, 0) {}
void spa::StmtlstStatementsStore::Set(Index<kStmtLst> stmtlst_index,
                                      std::vector<Index<kStmt>> &&stmtlst) {
    std::transform(stmtlst.begin(), stmtlst.end(),
                   stmtlst_to_statements[stmtlst_index.value].begin(),
                   [](Index<kStmt> i) { return i.value; });
    for (auto &i : stmtlst) {
        statement_to_stmtlst[i.value] = stmtlst_index.value;
    }
}

Index<kStmtLst> spa::StmtlstStatementsStore::GetStmtlst(
        Index<kStmt> stmt_no) const {
    return Index<kStmtLst>(statement_to_stmtlst[stmt_no.value]);
}
std::vector<Index<kStmt>> spa::StmtlstStatementsStore::GetStatements(
        Index<kStmtLst> stmtlst_index) {
    std::vector<Index<kStmt>> statements;
    statements.reserve(stmtlst_to_statements[stmtlst_index.value].size());

    std::transform(stmtlst_to_statements[stmtlst_index.value].begin(),
                   stmtlst_to_statements[stmtlst_index.value].begin(),
                   std::back_inserter(statements),
                   [](int i) { return Index<kStmt>(i); });
    return statements;
}
}  // namespace spa