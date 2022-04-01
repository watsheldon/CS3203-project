#include "stmtlst_statements_store.h"

#include <numeric>
#include <vector>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "common/index.h"

namespace spa {
StmtlstStatementsStore::StmtlstStatementsStore(std::size_t stmtlst_count,
                                               std::size_t stmt_count)
        : stmtlst_to_statements_(stmtlst_count + 1),
          statement_to_stmtlst_(stmt_count + 1) {}
void StmtlstStatementsStore::Set(int stmtlst_index, std::vector<int> stmtlst) {
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
std::size_t StmtlstStatementsStore::GetStmtlstSize(int stmt_no) const {
    return stmtlst_to_statements_[statement_to_stmtlst_[stmt_no].stmtlst_index]
            .size();
}
StmtProperties StmtlstStatementsStore::GetStmtProperties(int stmt_no) const {
    return statement_to_stmtlst_[stmt_no];
}
const std::vector<int> &StmtlstStatementsStore::GetStatements(
        int stmtlst_index) const {
    return stmtlst_to_statements_[stmtlst_index];
}
bool StmtlstStatementsStore::ExistFollows(Index<ArgPos::kFirst> first,
                                          Index<ArgPos::kSecond> second) const {
    if (first >= second || second + 1 > statement_to_stmtlst_.size()) {
        return false;
    }
    return GetStmtRelativePos(first) + 1 == GetStmtRelativePos(second) &&
           GetStmtlst(first) == GetStmtlst(second);
}
bool StmtlstStatementsStore::ExistFollowsT(
        Index<ArgPos::kFirst> first, Index<ArgPos::kSecond> second) const {
    if (first >= second || second + 1 > statement_to_stmtlst_.size()) {
        return false;
    }
    return GetStmtlst(first) == GetStmtlst(second);
}
bool StmtlstStatementsStore::ExistFollows(
        Index<ArgPos::kFirst> first_stmt) const {
    if (first_stmt + 2 > statement_to_stmtlst_.size()) {
        return false;
    }
    return GetStmtRelativePos(first_stmt) + 1 < GetStmtlstSize(first_stmt);
}
bool StmtlstStatementsStore::ExistFollows(
        Index<ArgPos::kSecond> second_stmt) const {
    // boundary check
    if (second_stmt + 1 > statement_to_stmtlst_.size()) {
        return false;
    }
    return GetStmtRelativePos(second_stmt) > 0;
}
bool StmtlstStatementsStore::ExistFollows() const {
    return GetFollowColumnSize() > 0;
}
std::vector<int> StmtlstStatementsStore::GetFollowsWildcard() const {
    std::vector<int> followers;
    followers.reserve(GetFollowColumnSize());
    for (auto stmtlst = ++stmtlst_to_statements_.begin();
         stmtlst != stmtlst_to_statements_.end(); ++stmtlst) {
        followers.insert(followers.end(), ++stmtlst->begin(), stmtlst->end());
    }
    return followers;
}
StmtNo StmtlstStatementsStore::GetFollowsSecondArg(
        Index<ArgPos::kFirst> first_stmt) const {
    if (first_stmt + 2 > statement_to_stmtlst_.size()) return 0;
    int second_pos = GetStmtRelativePos(first_stmt) + 1;
    if (second_pos == GetStmtlstSize(first_stmt)) return 0;
    int index = GetStmtlst(first_stmt);
    const std::vector<StmtNo> &stmts = GetStatements(index);
    return stmts[second_pos];
}
std::vector<int> StmtlstStatementsStore::GetFollowsTSecondArg(
        Index<ArgPos::kFirst> first_stmt) const {
    if (first_stmt + 2 > statement_to_stmtlst_.size()) return {};
    int start_index = GetStmtRelativePos(first_stmt) + 1;
    if (start_index == GetStmtlstSize(first_stmt)) return {};
    int index = GetStmtlst(first_stmt);
    const auto &stmts = GetStatements(index);
    return {stmts.begin() + start_index, stmts.end()};
}
std::vector<int> StmtlstStatementsStore::GetFollowedByWildcard() const {
    std::vector<int> followees;
    followees.reserve(GetFollowColumnSize());
    for (auto stmtlst = ++stmtlst_to_statements_.begin();
         stmtlst != stmtlst_to_statements_.end(); ++stmtlst) {
        followees.insert(followees.end(), stmtlst->begin(), --stmtlst->end());
    }
    return followees;
}
StmtNo StmtlstStatementsStore::GetFollowsFirstArg(
        Index<ArgPos::kSecond> second_stmt) const {
    if (second_stmt + 1 > statement_to_stmtlst_.size()) return 0;
    int relative_pos = GetStmtRelativePos(second_stmt);
    if (relative_pos == 0) return 0;
    int index = GetStmtlst(second_stmt);
    const auto &stmts = GetStatements(index);
    return stmts[relative_pos - 1];
}
std::vector<int> StmtlstStatementsStore::GetFollowsTFirstArg(
        Index<ArgPos::kSecond> second_stmt) const {
    if (second_stmt + 1 > statement_to_stmtlst_.size()) return {};
    int pos = GetStmtRelativePos(second_stmt);
    if (pos == 0) return {};
    int index = GetStmtlst(second_stmt);
    const std::vector<StmtNo> &stmts = GetStatements(index);
    return {stmts.begin(), stmts.begin() + pos};
}
PairVec<int> StmtlstStatementsStore::GetFollowsPairs() const {
    PairVec<int> results;
    auto &[first, second] = results;
    first.reserve(GetFollowColumnSize()), second.reserve(GetFollowColumnSize());
    for (auto stmtlst = ++stmtlst_to_statements_.begin();
         stmtlst != stmtlst_to_statements_.end(); ++stmtlst) {
        first.insert(first.end(), stmtlst->begin(), --stmtlst->end());
        second.insert(second.end(), ++stmtlst->begin(), stmtlst->end());
    }
    return results;
}
PairVec<int> StmtlstStatementsStore::GetFollowsPairsT() const {
    static const std::size_t kColumnLength = std::transform_reduce(
            ++stmtlst_to_statements_.begin(), stmtlst_to_statements_.end(), 0ul,
            std::plus<>(), [](const auto &s) {
                auto n = s.size();
                return (n - 1) * (n) / 2;
            });
    PairVec<int> results;
    auto &[first, second] = results;
    first.reserve(kColumnLength), second.reserve(kColumnLength);
    for (const auto &stmtlst : stmtlst_to_statements_) {
        const auto size = stmtlst.size();
        for (int i = 0; i + 1 < size; ++i) {
            auto num_pairs = size - i - 1;
            first.resize(first.size() + num_pairs, stmtlst[i]);
            second.insert(second.end(), stmtlst.begin() + i + 1, stmtlst.end());
        }
    }
    return results;
}
std::size_t StmtlstStatementsStore::GetFollowColumnSize() const {
    static const std::size_t kColumnSize = std::transform_reduce(
            ++stmtlst_to_statements_.begin(), stmtlst_to_statements_.end(), 0ul,
            std::plus<>(), [](const auto &s) { return s.size() - 1; });
    return kColumnSize;
}
}  // namespace spa
