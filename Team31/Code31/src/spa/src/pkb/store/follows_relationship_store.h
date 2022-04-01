#ifndef SRC_SPA_SRC_PKB_STORE_FOLLOWS_RELATIONSHIP_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_FOLLOWS_RELATIONSHIP_STORE_H_

#include <set>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "follows_parent_relationship_base.h"
#include "stmtlst_parent_store.h"
#include "stmtlst_statements_store.h"
#include "type_statements_store.h"

namespace spa {
class FollowsRelationshipStore : public FollowsParentRelationshipBase {
  public:
    using FollowsParentRelationshipBase::FollowsParentRelationshipBase;
    [[nodiscard]] bool IsNonTransitive(StmtNo first_stmt,
                                       StmtNo second_stmt) const noexcept final;
    [[nodiscard]] bool IsTransitive(StmtNo first_stmt,
                                    StmtNo second_stmt) const noexcept final;
    [[nodiscard]] bool HasSecondValues(StmtNo first_stmt) const noexcept final;
    [[nodiscard]] bool HasFirstValues(StmtNo second_stmt) const noexcept final;
    [[nodiscard]] bool ExistRelationship() const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetFirstArg(
            StmtType return_type) const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetSecondArg(
            StmtType return_type) const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetSecondArg(
            StmtNo first_stmt, StmtType return_type) const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetSecondArgT(
            StmtNo first_stmt, StmtType return_type) const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetFirstArg(
            StmtNo stmt_no, StmtType return_type) const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetFirstArgT(
            StmtNo second_stmt, StmtType return_type) const noexcept final;
    [[nodiscard]] PairVec<StmtNo> GetBothArgs(
            StmtType first_type, StmtType second_type) const noexcept final;
    [[nodiscard]] PairVec<StmtNo> GetBothArgsT(
            StmtType first_type, StmtType second_type) const noexcept final;

  private:
    [[nodiscard]] PairVec<StmtNo> ExtractPairs(
            PairVec<StmtNo> results, StmtType first_type,
            StmtType second_type) const noexcept;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_PKB_STORE_FOLLOWS_RELATIONSHIP_STORE_H_
