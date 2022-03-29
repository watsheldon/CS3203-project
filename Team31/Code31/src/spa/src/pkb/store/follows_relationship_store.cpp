#include "follows_relationship_store.h"

#include <algorithm>
#include <iterator>
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
std::set<int> FollowsRelationshipStore::GetOneArg(
        ArgPos return_pos, StmtType return_type) const noexcept {
    auto results = return_pos == ArgPos::kFirst
                           ? stmtlst_stmt_.GetFollowedByWildcard()
                           : stmtlst_stmt_.GetFollowsWildcard();
    return Extract(std::move(results), return_type);
}
std::set<int> FollowsRelationshipStore::GetOneArg(
        Index<ArgPos::kFirst> first_stmt, StmtType return_type) const noexcept {
    auto results = stmtlst_stmt_.GetFollows(first_stmt);
    return Extract(std::move(results), return_type);
}
std::set<int> FollowsRelationshipStore::GetOneArgT(
        Index<ArgPos::kFirst> first_stmt, StmtType return_type) const noexcept {
    auto results = stmtlst_stmt_.GetFollowsT(first_stmt);
    return Extract(std::move(results), return_type);
}
std::set<int> FollowsRelationshipStore::GetOneArg(
        Index<ArgPos::kSecond> second_stmt,
        StmtType return_type) const noexcept {
    auto results = stmtlst_stmt_.GetFollows(second_stmt);
    return Extract(std::move(results), return_type);
}
std::set<int> FollowsRelationshipStore::GetOneArgT(
        Index<ArgPos::kSecond> second_stmt,
        StmtType return_type) const noexcept {
    auto results = stmtlst_stmt_.GetFollowsT(second_stmt);
    return Extract(std::move(results), return_type);
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
}  // namespace spa
