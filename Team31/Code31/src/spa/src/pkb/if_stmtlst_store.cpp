#include "if_stmtlst_store.h"

#include "knowledge_base.h"

namespace spa {
spa::IfStmtlstStore::IfStmtlstStore(size_t stmt, size_t stmtlst)
        : if_to_stmtlst(stmt, IfPairs()), stmtlst_to_if(stmtlst, 0) {}

void spa::IfStmtlstStore::Set(Index<kStmt> stmt_no, Index<kStmtLst> then_index,
                              Index<kStmtLst> else_index) {
    if_to_stmtlst[stmt_no.value].then_index = then_index.value;
    if_to_stmtlst[stmt_no.value].else_index = else_index.value;
    stmtlst_to_if[then_index.value] = stmt_no.value;
    stmtlst_to_if[else_index.value] = stmt_no.value;
}

Index<kStmt> spa::IfStmtlstStore::GetStmtNo(
        Index<kStmtLst> stmtlst_index) const {
    return Index<kStmt>(stmtlst_to_if.at(stmtlst_index.value));
}

Index<kStmtLst> spa::IfStmtlstStore::GetThenStmtlst(
        Index<kStmt> stmt_no) const {
    return Index<kStmtLst>(if_to_stmtlst.at(stmt_no.value).then_index);
}
Index<kStmtLst> spa::IfStmtlstStore::GetElseStmtlst(
        Index<kStmt> stmt_no) const {
    return Index<kStmtLst>(if_to_stmtlst.at(stmt_no.value).else_index);
}
IfIndices IfStmtlstStore::GetBothStmtlst(Index<kStmt> stmt_no) const {
    IfIndices indices;
    indices.then_index =
            Index<kStmtLst>(if_to_stmtlst.at(stmt_no.value).then_index);
    indices.else_index =
            Index<kStmtLst>(if_to_stmtlst.at(stmt_no.value).else_index);
    return indices;
}
}  // namespace spa
