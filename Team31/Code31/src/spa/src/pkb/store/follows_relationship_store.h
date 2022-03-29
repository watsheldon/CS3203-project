#ifndef SRC_SPA_SRC_PKB_STORE_FOLLOWS_RELATIONSHIP_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_FOLLOWS_RELATIONSHIP_STORE_H_

#include <iterator>
#include <set>
#include <vector>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "common/index.h"
#include "follows_parent_relationship_base.h"
#include "stmtlst_parent_store.h"
#include "stmtlst_statements_store.h"
#include "type_statements_store.h"

namespace spa {
class FollowsRelationshipStore : public FollowsParentRelationshipBase {
  public:
    using FollowsParentRelationshipBase::FollowsParentRelationshipBase;
    [[nodiscard]] bool IsNonTransitive(
            Index<ArgPos::kFirst> first_stmt,
            Index<ArgPos::kSecond> second_stmt) const noexcept final;
    [[nodiscard]] bool IsTransitive(
            Index<ArgPos::kFirst> first_stmt,
            Index<ArgPos::kSecond> second_stmt) const noexcept final;
    [[nodiscard]] bool HasSecondValues(
            Index<ArgPos::kFirst> first_stmt) const noexcept final;
    [[nodiscard]] bool HasFirstValues(
            Index<ArgPos::kSecond> second_stmt) const noexcept final;
    [[nodiscard]] bool ExistRelationship() const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetOneArg(
            ArgPos return_pos, StmtType return_type) const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetOneArg(
            Index<ArgPos::kFirst> first_stmt,
            StmtType return_type) const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetOneArgT(
            Index<ArgPos::kFirst> first_stmt,
            StmtType return_type) const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetOneArg(
            Index<ArgPos::kSecond> second_stmt,
            StmtType return_type) const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetOneArgT(
            Index<ArgPos::kSecond> second_stmt,
            StmtType return_type) const noexcept final;
    [[nodiscard]] PairVec<StmtNo> GetBothArgs(
            StmtType first_type, StmtType second_type) const noexcept final;
    [[nodiscard]] PairVec<StmtNo> GetBothArgsT(
            StmtType first_type, StmtType second_type) const noexcept final;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_PKB_STORE_FOLLOWS_RELATIONSHIP_STORE_H_