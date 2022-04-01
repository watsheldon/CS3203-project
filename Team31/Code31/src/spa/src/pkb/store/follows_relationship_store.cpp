#include "follows_relationship_store.h"

#include <set>
#include <utility>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "follows_parent_relationship_base.h"

namespace spa {
bool FollowsRelationshipStore::IsNonTransitive(
        Index<ArgPos::kFirst> first_stmt,
        Index<ArgPos::kSecond> second_stmt) const noexcept {
    return stmtlst_stmt_.ExistFollows(first_stmt, second_stmt);
}
bool FollowsRelationshipStore::IsTransitive(
        Index<ArgPos::kFirst> first_stmt,
        Index<ArgPos::kSecond> second_stmt) const noexcept {
    return stmtlst_stmt_.ExistFollowsT(first_stmt, second_stmt);
}
bool FollowsRelationshipStore::HasSecondValues(
        Index<ArgPos::kFirst> first_stmt) const noexcept {
    return stmtlst_stmt_.ExistFollows(first_stmt);
}
bool FollowsRelationshipStore::HasFirstValues(
        Index<ArgPos::kSecond> second_stmt) const noexcept {
    return stmtlst_stmt_.ExistFollows(second_stmt);
}
bool FollowsRelationshipStore::ExistRelationship() const noexcept {
    return stmtlst_stmt_.ExistFollows();
}
std::set<StmtNo> FollowsRelationshipStore::GetFirstArg(
        StmtType return_type) const noexcept {
    return Filter(stmtlst_stmt_.GetFollowedByWildcard(), return_type);
}
std::set<StmtNo> FollowsRelationshipStore::GetSecondArg(
        StmtType return_type) const noexcept {
    return Filter(stmtlst_stmt_.GetFollowsWildcard(), return_type);
}
std::set<int> FollowsRelationshipStore::GetSecondArg(
        Index<ArgPos::kFirst> stmt_no, StmtType return_type) const noexcept {
    StmtNo second_arg = stmtlst_stmt_.GetFollowsSecondArg(stmt_no);
    return second_arg == 0 ? std::set<StmtNo>{}
                           : Filter(second_arg, return_type);
}
std::set<int> FollowsRelationshipStore::GetSecondArgT(
        Index<ArgPos::kFirst> stmt_no, StmtType return_type) const noexcept {
    auto results = stmtlst_stmt_.GetFollowsTSecondArg(stmt_no);
    return Filter(results, return_type);
}
std::set<StmtNo> FollowsRelationshipStore::GetFirstArg(
        Index<ArgPos::kSecond> stmt_no, StmtType return_type) const noexcept {
    auto first_arg = stmtlst_stmt_.GetFollowsFirstArg(stmt_no);
    return first_arg == 0 ? std::set<StmtNo>{} : Filter(first_arg, return_type);
}
std::set<int> FollowsRelationshipStore::GetFirstArgT(
        Index<ArgPos::kSecond> stmt_no, StmtType return_type) const noexcept {
    auto results = stmtlst_stmt_.GetFollowsTFirstArg(stmt_no);
    return Filter(results, return_type);
}
PairVec<int> FollowsRelationshipStore::GetBothArgs(
        StmtType first_type, StmtType second_type) const noexcept {
    PairVec<int> results = stmtlst_stmt_.GetFollowsPairs();
    return ExtractPairs(std::move(results), first_type, second_type);
}
PairVec<int> FollowsRelationshipStore::GetBothArgsT(
        StmtType first_type, StmtType second_type) const noexcept {
    PairVec<int> results = stmtlst_stmt_.GetFollowsPairsT();
    return ExtractPairs(std::move(results), first_type, second_type);
}
PairVec<StmtNo> FollowsRelationshipStore::ExtractPairs(
        PairVec<StmtNo> results, StmtType first_type,
        StmtType second_type) const noexcept {
    if (first_type == StmtType::kAll && second_type == StmtType::kAll)
        return results;
    PairVec<int> extracted_results;
    auto &[first, second] = results;
    for (int i = 0; i < results.first.size(); ++i) {
        bool has_first = first_type == StmtType::kAll ||
                         first_type == type_stmt_.GetType(first[i]);
        bool has_second = second_type == StmtType::kAll ||
                          second_type == type_stmt_.GetType(second[i]);
        if (has_first && has_second) {
            extracted_results.first.emplace_back(first[i]);
            extracted_results.second.emplace_back(second[i]);
        }
    }
    return extracted_results;
}
}  // namespace spa
