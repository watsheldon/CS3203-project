#include "stmtlst_statements_store.h"

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "pkb/knowledge_base.h"

namespace spa {
StmtlstStatementsStore::StmtlstStatementsStore(size_t stmtlst_count,
                                               size_t stmt_count)
        : stmtlst_to_statements_(stmtlst_count + 1),
          statement_to_stmtlst_(stmt_count + 1) {}
void StmtlstStatementsStore::Set(int stmtlst_index,
                                 std::vector<int> &&stmtlst) {
    for (int i = 0; i < stmtlst.size(); ++i) {
        statement_to_stmtlst_[stmtlst[i]] = {stmtlst_index, i};
    }
    stmtlst_to_statements_[stmtlst_index] = std::move(stmtlst);
}

int StmtlstStatementsStore::GetStmtlst(int stmt_no) const {
    return statement_to_stmtlst_[stmt_no].stmtlst_index;
}
int StmtlstStatementsStore::GetStmtRelativePos(int stmt_no) const {
    return statement_to_stmtlst_[stmt_no].pos_in_stmtlst;
}
size_t StmtlstStatementsStore::GetStmtlstSize(int stmt_no) const {
    return stmtlst_to_statements_[statement_to_stmtlst_[stmt_no].stmtlst_index]
            .size();
}
StmtProperties StmtlstStatementsStore::GetStmtProperties(int stmt_no) const {
    return statement_to_stmtlst_[stmt_no];
}
std::vector<int> StmtlstStatementsStore::GetStatements(
        int stmtlst_index) const {
    return stmtlst_to_statements_[stmtlst_index];
}

bool StmtlstStatementsStore::ExistFollows(
        bool transitive, Index<ArgPos::kFirst> first_stmt,
        Index<ArgPos::kSecond> second_stmt) const {
    // boundary check
    if (first_stmt.value > statement_to_stmtlst_.size() - 2 ||
        second_stmt.value > statement_to_stmtlst_.size() - 1) {
        return false;
    }

    return (first_stmt.value < second_stmt.value) &&
           (GetStmtlst(first_stmt.value) == GetStmtlst(second_stmt.value)) &&
           (transitive || (GetStmtRelativePos(second_stmt.value) -
                                   GetStmtRelativePos(first_stmt.value) ==
                           1));
}
bool StmtlstStatementsStore::ExistFollows(
        Index<ArgPos::kFirst> first_stmt) const {
    if (first_stmt.value > statement_to_stmtlst_.size() - 2) {
        return false;
    }
    return GetStmtRelativePos(first_stmt.value) <
           GetStmtlstSize(first_stmt.value) - 1;
}
bool StmtlstStatementsStore::ExistFollows(
        Index<ArgPos::kSecond> second_stmt) const {
    // boundary check
    if (second_stmt.value > statement_to_stmtlst_.size() - 1) {
        return false;
    }
    return GetStmtRelativePos(second_stmt.value) > 0;
}
bool StmtlstStatementsStore::ExistFollows() const {
    return std::any_of(stmtlst_to_statements_.begin() + 1,
                       stmtlst_to_statements_.end(),
                       [](auto &stmtlst) { return stmtlst.size() > 1; });
}

std::vector<int> StmtlstStatementsStore::GetFollowsWildcard() const {
    std::vector<int> followers;
    for (auto &stmtlst : stmtlst_to_statements_) {
        for (int i = 1; i < stmtlst.size(); ++i) {
            followers.emplace_back(stmtlst[i]);
        }
    }
    return followers;
}
std::vector<int> StmtlstStatementsStore::GetFollowsFirst(
        bool transitive, Index<ArgPos::kFirst> first_stmt) const {
    int pos = GetStmtRelativePos(first_stmt.value);
    int index = GetStmtlst(first_stmt.value);
    const std::vector<int> &stmts = GetStatements(index);
    if (!transitive) {
        return {stmts[pos + 1]};
    }
    auto first = stmts.begin() + pos + 1;
    auto last = stmts.end();
    return {first, last};
}
std::vector<int> StmtlstStatementsStore::GetFollows(
        bool transitive, Index<ArgPos::kFirst> first_stmt) const {
    if (first_stmt.value > statement_to_stmtlst_.size() - 2) {
        return {};
    }
    if (first_stmt.value == 0) {  // (_,outputs)
        return GetFollowsWildcard();
    }
    return GetFollowsFirst(transitive, first_stmt);
}

std::vector<int> StmtlstStatementsStore::GetFollowedByWildcard() const {
    std::vector<int> followees;
    for (auto &stmtlst : stmtlst_to_statements_) {
        for (int i = 0; i < stmtlst.size() - 1; ++i) {
            followees.emplace_back(stmtlst[i]);
        }
    }
    return followees;
}
std::vector<int> StmtlstStatementsStore::GetFollowedBySecond(
        bool transitive, Index<ArgPos::kSecond> second_stmt) const {
    int pos = GetStmtRelativePos(second_stmt.value);
    int index = GetStmtlst(second_stmt.value);
    const std::vector<int> &stmts = GetStatements(index);
    if (!transitive) {
        return {stmts[pos - 1]};
    }
    auto first = stmts.begin();
    auto last = stmts.begin() + pos;
    return {first, last};
}
std::vector<int> StmtlstStatementsStore::GetFollows(
        bool transitive, Index<ArgPos::kSecond> second_stmt) const {
    if (second_stmt.value > statement_to_stmtlst_.size() - 1) {
        return {};
    }
    if (second_stmt.value == 0) {  // (outputs, _)
        return GetFollowedByWildcard();
    }
    return GetFollowedBySecond(transitive, second_stmt);
}

void StmtlstStatementsStore::AddPairs(
        const std::vector<int> &stmtlst,
        std::vector<std::pair<int, int>> results) const {
    for (int i = 0; i < stmtlst.size() - 1; ++i) {
        for (int j = i + 1; j < stmtlst.size(); ++j) {
            results.emplace_back(std::make_pair(stmtlst[i], stmtlst[j]));
        }
    }
}
std::vector<std::pair<int, int>> StmtlstStatementsStore::GetTransitivePairs()
        const {
    std::vector<std::pair<int, int>> results;
    for (auto &stmtlst : stmtlst_to_statements_) {
        AddPairs(stmtlst, results);
    }
    return results;
}
std::vector<std::pair<int, int>> StmtlstStatementsStore::GetNonTransitivePairs()
        const {
    std::vector<std::pair<int, int>> results;
    for (auto &stmtlst : stmtlst_to_statements_) {
        for (int i = 0; i < stmtlst.size() - 1; ++i) {
            results.emplace_back(std::make_pair(stmtlst[i], stmtlst[i + 1]));
        }
    }
    return results;
}
std::vector<std::pair<int, int>> StmtlstStatementsStore::GetFollowsPairs(
        bool transitive) const {
    if (!transitive) {
        return GetTransitivePairs();
    }
    return GetNonTransitivePairs();
}

}  // namespace spa
