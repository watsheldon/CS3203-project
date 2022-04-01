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
std::set<StmtNo> FollowsParentRelationshipBase::Filter(
        StmtNo result, StmtType return_type) const noexcept {
    if (return_type == StmtType::kAll) return {result};
    return type_stmt_.GetType(result) == return_type ? std::set<StmtNo>{result}
                                                     : std::set<StmtNo>{};
}
std::set<int> FollowsParentRelationshipBase::Filter(
        const std::vector<int> &results, StmtType return_type) const noexcept {
    if (return_type == StmtType::kAll) return {results.begin(), results.end()};
    std::set<int> filtered_results;
    std::copy_if(results.begin(), results.end(),
                 std::inserter(filtered_results, filtered_results.end()),
                 [this, return_type](StmtNo i) {
                     return type_stmt_.GetType(i) == return_type;
                 });
    return filtered_results;
}
}  // namespace spa
