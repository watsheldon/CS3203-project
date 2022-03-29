#include "follows_relationship_store.h"

#include <algorithm>
#include <iterator>
#include <set>
#include <utility>
#include <vector>

#include "common/aliases.h"
#include "common/entity_type_enum.h"

namespace spa {
FollowsRelationshipStore::FollowsRelationshipStore(StoreRefs refs) noexcept
        : type_stmt_(refs.type_stmt),
          stmtlst_parent_(refs.stmtlst_parent),
          stmtlst_stmt_(refs.stmtlst_stmt) {}
bool FollowsRelationshipStore::ExistFollows(
        Index<ArgPos::kFirst> first_stmt,
        Index<ArgPos::kSecond> second_stmt) const noexcept {
    return stmtlst_stmt_.ExistFollows(first_stmt, second_stmt);
}
bool FollowsRelationshipStore::ExistFollowsT(
        Index<ArgPos::kFirst> first_stmt,
        Index<ArgPos::kSecond> second_stmt) const noexcept {
    return stmtlst_stmt_.ExistFollowsT(first_stmt, second_stmt);
}
bool FollowsRelationshipStore::ExistFollows(
        Index<ArgPos::kFirst> first_stmt) const noexcept {
    return stmtlst_stmt_.ExistFollows(first_stmt);
}
bool FollowsRelationshipStore::ExistFollows(
        Index<ArgPos::kSecond> second_stmt) const noexcept {
    return stmtlst_stmt_.ExistFollows(second_stmt);
}
bool FollowsRelationshipStore::ExistFollows() const noexcept {
    return stmtlst_stmt_.ExistFollows();
}
std::set<int> FollowsRelationshipStore::GetFollows(
        ArgPos return_pos, StmtType return_type) const noexcept {
    auto results = return_pos == ArgPos::kFirst
                           ? stmtlst_stmt_.GetFollowedByWildcard()
                           : stmtlst_stmt_.GetFollowsWildcard();
    return Extract(std::move(results), return_type);
}
std::set<int> FollowsRelationshipStore::GetFollows(
        Index<ArgPos::kFirst> first_stmt, StmtType return_type) const noexcept {
    auto results = stmtlst_stmt_.GetFollows(first_stmt);
    return Extract(std::move(results), return_type);
}
std::set<int> FollowsRelationshipStore::GetFollowsT(
        Index<ArgPos::kFirst> first_stmt, StmtType return_type) const noexcept {
    auto results = stmtlst_stmt_.GetFollowsT(first_stmt);
    return Extract(std::move(results), return_type);
}
std::set<int> FollowsRelationshipStore::GetFollows(
        Index<ArgPos::kSecond> second_stmt,
        StmtType return_type) const noexcept {
    auto results = stmtlst_stmt_.GetFollows(second_stmt);
    return Extract(std::move(results), return_type);
}
std::set<int> FollowsRelationshipStore::GetFollowsT(
        Index<ArgPos::kSecond> second_stmt,
        StmtType return_type) const noexcept {
    auto results = stmtlst_stmt_.GetFollowsT(second_stmt);
    return Extract(std::move(results), return_type);
}
PairVec<int> FollowsRelationshipStore::GetFollowsPairs(
        StmtType first_type, StmtType second_type) const noexcept {
    PairVec<int> results = stmtlst_stmt_.GetFollowsPairs();
    return ExtractPairs(std::move(results), first_type, second_type);
}

PairVec<int> FollowsRelationshipStore::GetFollowsPairsT(
        StmtType first_type, StmtType second_type) const noexcept {
    PairVec<int> results = stmtlst_stmt_.GetFollowsPairsT();
    return ExtractPairs(std::move(results), first_type, second_type);
}
std::set<int> FollowsRelationshipStore::Extract(
        std::vector<int> results, StmtType return_type) const noexcept {
    if (return_type == StmtType::kAll) {
        return {results.begin(), results.end()};
    }
    std::set<int> extracted_results;
    std::copy_if(results.begin(), results.end(),
                 std::inserter(extracted_results, extracted_results.end()),
                 [this, return_type](int i) {
                     return type_stmt_.GetType(i) == return_type;
                 });
    return extracted_results;
}
PairVec<int> FollowsRelationshipStore::ExtractPairs(
        PairVec<int> results, StmtType first, StmtType second) const noexcept {
    if (first == StmtType::kAll && second == StmtType::kAll) return results;
    PairVec<int> extracted_results;
    for (int i = 0; i < results.first.size(); ++i) {
        bool has_first = first == StmtType::kAll ||
                         first == type_stmt_.GetType(results.first[i]);
        bool has_second = second == StmtType::kAll ||
                          second == type_stmt_.GetType(results.second[i]);
        if (has_first && has_second) {
            extracted_results.first.emplace_back(results.first[i]);
            extracted_results.second.emplace_back(results.second[i]);
        }
    }
    return extracted_results;
}
}  // namespace spa
