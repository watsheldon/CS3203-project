#include "follows_relationship_store.h"

#include <set>
#include <utility>

#include "common/aliases.h"
#include "common/entity_type_enum.h"

namespace spa {
bool FollowsRelationshipStore::IsNonTransitive(
        StmtNo first_stmt, StmtNo second_stmt) const noexcept {
    return stmtlst_stmt_.ExistFollows(first_stmt, second_stmt);
}
bool FollowsRelationshipStore::IsTransitive(StmtNo parent,
                                            StmtNo child) const noexcept {
    return stmtlst_stmt_.ExistFollowsT(parent, child);
}
bool FollowsRelationshipStore::HasSecondValues(
        StmtNo first_stmt) const noexcept {
    return stmtlst_stmt_.ExistFollowsSecondArg(first_stmt);
}
bool FollowsRelationshipStore::HasFirstValues(
        StmtNo second_stmt) const noexcept {
    return stmtlst_stmt_.ExistFollowsFirstArg(second_stmt);
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
std::set<StmtNo> FollowsRelationshipStore::GetSecondArg(
        StmtNo parent, StmtType return_type) const noexcept {
    StmtNo second_arg = stmtlst_stmt_.GetFollowsSecondArg(parent);
    return second_arg == 0 ? std::set<StmtNo>{}
                           : Filter(second_arg, return_type);
}
std::set<StmtNo> FollowsRelationshipStore::GetSecondArgT(
        StmtNo stmt_no, StmtType return_type) const noexcept {
    auto results = stmtlst_stmt_.GetFollowsTSecondArg(stmt_no);
    return Filter(results, return_type);
}
std::set<StmtNo> FollowsRelationshipStore::GetFirstArg(
        StmtNo stmt_no, StmtType return_type) const noexcept {
    auto first_arg = stmtlst_stmt_.GetFollowsFirstArg(stmt_no);
    return first_arg == 0 ? std::set<StmtNo>{} : Filter(first_arg, return_type);
}
std::set<StmtNo> FollowsRelationshipStore::GetFirstArgT(
        StmtNo stmt_no, StmtType return_type) const noexcept {
    auto results = stmtlst_stmt_.GetFollowsTFirstArg(stmt_no);
    return Filter(results, return_type);
}
PairVec<StmtNo> FollowsRelationshipStore::GetBothArgs(
        StmtType first_type, StmtType second_type) const noexcept {
    PairVec<StmtNo> results = stmtlst_stmt_.GetFollowsPairs();
    return ExtractPairs(std::move(results), first_type, second_type);
}
PairVec<StmtNo> FollowsRelationshipStore::GetBothArgsT(
        StmtType first_type, StmtType second_type) const noexcept {
    PairVec<StmtNo> results = stmtlst_stmt_.GetFollowsPairsT();
    return ExtractPairs(std::move(results), first_type, second_type);
}
PairVec<StmtNo> FollowsRelationshipStore::ExtractPairs(
        PairVec<StmtNo> results, StmtType first_type,
        StmtType second_type) const noexcept {
    if (first_type == StmtType::kAll && second_type == StmtType::kAll)
        return results;
    PairVec<StmtNo> extracted_results;
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
