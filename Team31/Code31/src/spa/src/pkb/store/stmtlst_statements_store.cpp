#include "stmtlst_statements_store.h"

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "pkb/knowledge_base.h"

namespace spa {
spa::StmtlstStatementsStore::StmtlstStatementsStore(size_t stmtlst_count,
                                                    size_t stmt_count)
        : stmtlst_to_statements_(stmtlst_count + 1),
          statement_to_stmtlst_(stmt_count + 1) {}
void spa::StmtlstStatementsStore::Set(int stmtlst_index,
                                      std::vector<int> &&stmtlst) {
    stmtlst_to_statements_[stmtlst_index] = stmtlst;
    for (int i = 1; i < stmtlst.size(); ++i) {
        statement_to_stmtlst_[stmtlst[i]] = {
                stmtlst_index, i, static_cast<int>(stmtlst.size()) - 1};
    }
}

int spa::StmtlstStatementsStore::GetStmtlstIndex(int stmt_no) const {
    return statement_to_stmtlst_[stmt_no].stmtlst_index;
}
int spa::StmtlstStatementsStore::GetStmtlstPos(int stmt_no) const {
    return statement_to_stmtlst_[stmt_no].pos_in_stmtlst;
}
int spa::StmtlstStatementsStore::GetStmtlstSize(int stmt_no) const {
    return statement_to_stmtlst_[stmt_no].stmtlst_size;
}
StmtlstIndexPosSize spa::StmtlstStatementsStore::GetIndexPosSize(
        int stmt_no) const {
    return statement_to_stmtlst_[stmt_no];
}
std::vector<int> spa::StmtlstStatementsStore::GetStatements(
        int stmtlst_index) const {
    return stmtlst_to_statements_[stmtlst_index];
}

bool spa::StmtlstStatementsStore::Follows() const {
    return std::any_of(stmtlst_to_statements_.begin() + 1,
                       stmtlst_to_statements_.end(),
                       [](auto &stmtlst) { return stmtlst.size() > 1; });
}
bool spa::StmtlstStatementsStore::Follows(
        Index<ArgPos::kFirst> first_stmt) const {
    return GetStmtlstPos(first_stmt.value) < GetStmtlstSize(first_stmt.value);
}
bool spa::StmtlstStatementsStore::Follows(
        Index<ArgPos::kSecond> second_stmt) const {
    return GetStmtlstPos(second_stmt.value) > 1;
}
bool spa::StmtlstStatementsStore::Follows(
        bool transitive, Index<ArgPos::kFirst> first_stmt,
        Index<ArgPos::kSecond> second_stmt) const {
    return (first_stmt.value < second_stmt.value) &&
           (GetStmtlstIndex(first_stmt.value) ==
            GetStmtlstIndex(second_stmt.value)) &&
           (transitive || (GetStmtlstPos(second_stmt.value) -
                                   GetStmtlstPos(first_stmt.value) ==
                           1));
}

std::vector<int> StmtlstStatementsStore::GetFollows(
        Index<ArgPos::kFirst> first_stmt) const {
    assert(first_stmt.value == 0);
    std::vector<int> followers;
    for (auto &stmtlst : stmtlst_to_statements_) {
        for (int i = 2; i < stmtlst.size(); ++i) {
            followers.emplace_back(stmtlst[i]);
        }
    }
    std::sort(followers.begin(), followers.end());
    return followers;
}
std::vector<int> StmtlstStatementsStore::GetFollows(
        bool transitive, Index<ArgPos::kFirst> first_stmt) const {
    int end = GetStmtlstSize(first_stmt.value);
    int pos = GetStmtlstPos(first_stmt.value);
    if (pos == end) {
        return {};
    }
    int index = GetStmtlstIndex(first_stmt.value);
    const std::vector<int> &stmts = GetStatements(index);
    if (!transitive) {
        return {stmts[pos + 1]};
    }
    auto first = stmts.begin() + pos + 1;
    auto last = stmts.begin() + end + 1;
    return {first, last};
}
std::vector<int> StmtlstStatementsStore::GetFollows(
        Index<ArgPos::kSecond> second_stmt) const {
    assert(second_stmt.value == 0);
    std::vector<int> followees;
    for (auto &stmtlst : stmtlst_to_statements_) {
        for (int i = 1; i < stmtlst.size() - 1; ++i) {
            followees.emplace_back(stmtlst[i]);
        }
    }
    std::sort(followees.begin(), followees.end());
    return followees;
}
std::vector<int> StmtlstStatementsStore::GetFollows(
        bool transitive, Index<ArgPos::kSecond> second_stmt) const {
    int pos = GetStmtlstPos(second_stmt.value);
    if (pos == 1) {
        return {};
    }
    int index = GetStmtlstIndex(second_stmt.value);
    const std::vector<int> &stmts = GetStatements(index);
    if (!transitive) {
        return {stmts[pos - 1]};
    }
    auto first = stmts.begin() + 1;
    auto last = stmts.begin() + pos;
    return {first, last};
}

}  // namespace spa
