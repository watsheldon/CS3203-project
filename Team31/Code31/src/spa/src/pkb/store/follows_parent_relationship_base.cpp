#include "follows_parent_relationship_base.h"

#include <algorithm>
#include <iterator>
#include <set>
#include <vector>

#include "common/aliases.h"
#include "common/entity_type_enum.h"

namespace spa {
FollowsParentRelationshipBase::FollowsParentRelationshipBase(
        FollowsParentRelationshipBase::StoreRefs refs) noexcept
        : type_stmt_(refs.type_stmt),
          stmtlst_parent_(refs.stmtlst_parent),
          stmtlst_stmt_(refs.stmtlst_stmt) {}
std::set<int> FollowsParentRelationshipBase::Extract(
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
PairVec<int> FollowsParentRelationshipBase::ExtractPairs(
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
