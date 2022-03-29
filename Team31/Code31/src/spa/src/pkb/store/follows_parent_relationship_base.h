#ifndef SRC_SPA_SRC_PKB_STORE_FOLLOWS_PARENT_RELATIONSHIP_BASE_H_
#define SRC_SPA_SRC_PKB_STORE_FOLLOWS_PARENT_RELATIONSHIP_BASE_H_

#include <set>
#include <vector>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "common/index.h"
#include "stmtlst_parent_store.h"
#include "stmtlst_statements_store.h"
#include "type_statements_store.h"

namespace spa {
class FollowsParentRelationshipBase {
  public:
    struct StoreRefs {
        const TypeStatementsStore &type_stmt;
        const StmtlstParentStore &stmtlst_parent;
        const StmtlstStatementsStore &stmtlst_stmt;
    };
    explicit FollowsParentRelationshipBase(StoreRefs refs) noexcept;
    [[nodiscard]] virtual bool IsNonTransitive(
            Index<ArgPos::kFirst> first_stmt,
            Index<ArgPos::kSecond> second_stmt) const noexcept = 0;
    [[nodiscard]] virtual bool IsTransitive(
            Index<ArgPos::kFirst> first_stmt,
            Index<ArgPos::kSecond> second_stmt) const noexcept = 0;
    [[nodiscard]] virtual bool HasSecondValues(
            Index<ArgPos::kFirst> first_stmt) const noexcept = 0;
    [[nodiscard]] virtual bool HasFirstValues(
            Index<ArgPos::kSecond> second_stmt) const noexcept = 0;
    [[nodiscard]] virtual bool ExistRelationship() const noexcept = 0;
    [[nodiscard]] virtual std::set<StmtNo> GetOneArg(
            ArgPos return_pos, StmtType return_type) const noexcept = 0;
    [[nodiscard]] virtual std::set<StmtNo> GetOneArg(
            Index<ArgPos::kFirst> first_stmt,
            StmtType return_type) const noexcept = 0;
    [[nodiscard]] virtual std::set<StmtNo> GetOneArgT(
            Index<ArgPos::kFirst> first_stmt,
            StmtType return_type) const noexcept = 0;
    [[nodiscard]] virtual std::set<StmtNo> GetOneArg(
            Index<ArgPos::kSecond> second_stmt,
            StmtType return_type) const noexcept = 0;
    [[nodiscard]] virtual std::set<StmtNo> GetOneArgT(
            Index<ArgPos::kSecond> second_stmt,
            StmtType return_type) const noexcept = 0;
    [[nodiscard]] virtual PairVec<StmtNo> GetBothArgs(
            StmtType first_type, StmtType second_type) const noexcept = 0;
    [[nodiscard]] virtual PairVec<StmtNo> GetBothArgsT(
            StmtType first_type, StmtType second_type) const noexcept = 0;

  protected:
    const TypeStatementsStore &type_stmt_;
    const StmtlstParentStore &stmtlst_parent_;
    const StmtlstStatementsStore &stmtlst_stmt_;

    [[nodiscard]] std::set<StmtNo> Extract(std::vector<StmtNo> results,
                                           StmtType return_type) const noexcept;
    [[nodiscard]] PairVec<StmtNo> ExtractPairs(PairVec<StmtNo> results,
                                               StmtType first,
                                               StmtType second) const noexcept;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_FOLLOWS_PARENT_RELATIONSHIP_BASE_H_
