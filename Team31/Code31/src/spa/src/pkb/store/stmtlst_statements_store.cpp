#include "stmtlst_statements_store.h"

#include <numeric>
#include <vector>

#include "common/aliases.h"

namespace spa {
StmtlstStatementsStore::StmtlstStatementsStore(std::size_t stmtlst_count,
                                               std::size_t stmt_count) noexcept
        : stmtlst_to_statements_(stmtlst_count + 1),
          statement_to_stmtlst_(stmt_count + 1) {}
void StmtlstStatementsStore::Set(StmtLstIndex stmtlst_index,
                                 std::vector<StmtNo> stmtlst) {
    for (int i = 0; i < stmtlst.size(); ++i) {
        statement_to_stmtlst_[stmtlst[i]] = {stmtlst_index, i};
    }
    stmtlst_to_statements_[stmtlst_index] = std::move(stmtlst);
}
StmtLstIndex StmtlstStatementsStore::GetStmtlst(StmtNo stmt_no) const noexcept {
    return statement_to_stmtlst_[stmt_no].stmtlst_index;
}
int StmtlstStatementsStore::GetStmtRelativePos(StmtNo stmt_no) const noexcept {
    return statement_to_stmtlst_[stmt_no].pos_in_stmtlst;
}
std::size_t StmtlstStatementsStore::GetStmtlstSize(
        StmtNo stmt_no) const noexcept {
    return stmtlst_to_statements_[statement_to_stmtlst_[stmt_no].stmtlst_index]
            .size();
}
StmtProperties StmtlstStatementsStore::GetStmtProperties(
        StmtNo stmt_no) const noexcept {
    return statement_to_stmtlst_[stmt_no];
}
const std::vector<StmtNo> &StmtlstStatementsStore::GetStatements(
        StmtLstIndex stmtlst_index) const noexcept {
    return stmtlst_to_statements_[stmtlst_index];
}
bool StmtlstStatementsStore::ExistFollows(StmtNo first,
                                          StmtNo second) const noexcept {
    if (first >= second || second + 1 > statement_to_stmtlst_.size()) {
        return false;
    }
    return GetStmtRelativePos(first) + 1 == GetStmtRelativePos(second) &&
           GetStmtlst(first) == GetStmtlst(second);
}
bool StmtlstStatementsStore::ExistFollowsT(StmtNo first,
                                           StmtNo second) const noexcept {
    if (first >= second || second + 1 > statement_to_stmtlst_.size()) {
        return false;
    }
    return GetStmtlst(first) == GetStmtlst(second);
}
bool StmtlstStatementsStore::ExistFollowsSecondArg(
        StmtNo first_stmt) const noexcept {
    if (first_stmt + 2 > statement_to_stmtlst_.size()) {
        return false;
    }
    return GetStmtRelativePos(first_stmt) + 1 < GetStmtlstSize(first_stmt);
}
bool StmtlstStatementsStore::ExistFollowsFirstArg(
        StmtNo second_stmt) const noexcept {
    // boundary check
    if (second_stmt + 1 > statement_to_stmtlst_.size()) {
        return false;
    }
    return GetStmtRelativePos(second_stmt) > 0;
}
bool StmtlstStatementsStore::ExistFollows() const noexcept {
    return GetFollowColumnSize() > 0;
}
std::vector<StmtNo> StmtlstStatementsStore::GetFollowsWildcard()
        const noexcept {
    std::vector<StmtNo> followers;
    followers.reserve(GetFollowColumnSize());
    for (auto stmtlst = ++stmtlst_to_statements_.begin();
         stmtlst != stmtlst_to_statements_.end(); ++stmtlst) {
        followers.insert(followers.end(), ++stmtlst->begin(), stmtlst->end());
    }
    return followers;
}
StmtNo StmtlstStatementsStore::GetFollowsSecondArg(
        StmtNo first_stmt) const noexcept {
    if (first_stmt + 2 > statement_to_stmtlst_.size()) return 0;
    int second_pos = GetStmtRelativePos(first_stmt) + 1;
    if (second_pos == GetStmtlstSize(first_stmt)) return 0;
    StmtLstIndex index = GetStmtlst(first_stmt);
    const std::vector<StmtNo> &stmts = GetStatements(index);
    return stmts[second_pos];
}
std::vector<StmtNo> StmtlstStatementsStore::GetFollowsTSecondArg(
        StmtNo first_stmt) const noexcept {
    if (first_stmt + 2 > statement_to_stmtlst_.size()) return {};
    int start_index = GetStmtRelativePos(first_stmt) + 1;
    if (start_index == GetStmtlstSize(first_stmt)) return {};
    StmtLstIndex index = GetStmtlst(first_stmt);
    const auto &stmts = GetStatements(index);
    return {stmts.begin() + start_index, stmts.end()};
}
std::vector<StmtNo> StmtlstStatementsStore::GetFollowedByWildcard()
        const noexcept {
    std::vector<StmtNo> followees;
    followees.reserve(GetFollowColumnSize());
    for (auto stmtlst = ++stmtlst_to_statements_.begin();
         stmtlst != stmtlst_to_statements_.end(); ++stmtlst) {
        followees.insert(followees.end(), stmtlst->begin(), --stmtlst->end());
    }
    return followees;
}
StmtNo StmtlstStatementsStore::GetFollowsFirstArg(
        StmtNo second_stmt) const noexcept {
    if (second_stmt + 1 > statement_to_stmtlst_.size()) return 0;
    int relative_pos = GetStmtRelativePos(second_stmt);
    if (relative_pos == 0) return 0;
    StmtLstIndex index = GetStmtlst(second_stmt);
    const auto &stmts = GetStatements(index);
    return stmts[relative_pos - 1];
}
std::vector<StmtNo> StmtlstStatementsStore::GetFollowsTFirstArg(
        StmtNo second_stmt) const noexcept {
    if (second_stmt + 1 > statement_to_stmtlst_.size()) return {};
    int pos = GetStmtRelativePos(second_stmt);
    if (pos == 0) return {};
    StmtLstIndex index = GetStmtlst(second_stmt);
    const std::vector<StmtNo> &stmts = GetStatements(index);
    return {stmts.begin(), stmts.begin() + pos};
}
PairVec<StmtNo> StmtlstStatementsStore::GetFollowsPairs() const noexcept {
    PairVec<StmtNo> results;
    auto &[first, second] = results;
    first.reserve(GetFollowColumnSize()), second.reserve(GetFollowColumnSize());
    for (auto stmtlst = ++stmtlst_to_statements_.begin();
         stmtlst != stmtlst_to_statements_.end(); ++stmtlst) {
        first.insert(first.end(), stmtlst->begin(), --stmtlst->end());
        second.insert(second.end(), ++stmtlst->begin(), stmtlst->end());
    }
    return results;
}
PairVec<StmtNo> StmtlstStatementsStore::GetFollowsPairsT() const noexcept {
    static const std::size_t kColumnLength = std::transform_reduce(
            ++stmtlst_to_statements_.begin(), stmtlst_to_statements_.end(), 0ul,
            std::plus<>(), [](const auto &s) {
                auto n = s.size();
                return (n - 1) * (n) / 2;
            });
    PairVec<StmtNo> results;
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
std::size_t StmtlstStatementsStore::GetFollowColumnSize() const noexcept {
    static const std::size_t kColumnSize = std::transform_reduce(
            ++stmtlst_to_statements_.begin(), stmtlst_to_statements_.end(), 0ul,
            std::plus<>(), [](const auto &s) { return s.size() - 1; });
    return kColumnSize;
}
}  // namespace spa
