#ifndef SRC_SPA_SRC_PKB_STORE_FOLLOWS_RELATIONSHIP_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_FOLLOWS_RELATIONSHIP_STORE_H_

#include <iterator>
#include <set>
#include <vector>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "common/index.h"
#include "stmtlst_parent_store.h"
#include "stmtlst_statements_store.h"
#include "type_statements_store.h"

namespace spa {
class FollowsRelationshipStore {
  public:
    struct StoreRefs {
        const TypeStatementsStore &type_stmt;
        const StmtlstParentStore &stmtlst_parent;
        const StmtlstStatementsStore &stmtlst_stmt;
    };
    explicit FollowsRelationshipStore(StoreRefs refs) noexcept;
    [[nodiscard]] bool ExistFollows(
            Index<ArgPos::kFirst> first_stmt,
            Index<ArgPos::kSecond> second_stmt) const noexcept;
    [[nodiscard]] bool ExistFollowsT(
            Index<ArgPos::kFirst> first_stmt,
            Index<ArgPos::kSecond> second_stmt) const noexcept;
    [[nodiscard]] bool ExistFollows(
            Index<ArgPos::kFirst> first_stmt) const noexcept;
    [[nodiscard]] bool ExistFollows(
            Index<ArgPos::kSecond> second_stmt) const noexcept;
    [[nodiscard]] bool ExistFollows() const noexcept;
    [[nodiscard]] std::set<StmtNo> GetFollows(
            ArgPos return_pos, StmtType return_type) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetFollows(
            Index<ArgPos::kFirst> first_stmt,
            StmtType return_type) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetFollowsT(
            Index<ArgPos::kFirst> first_stmt,
            StmtType return_type) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetFollows(
            Index<ArgPos::kSecond> second_stmt,
            StmtType return_type) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetFollowsT(
            Index<ArgPos::kSecond> second_stmt,
            StmtType return_type) const noexcept;
    [[nodiscard]] PairVec<StmtNo> GetFollowsPairs(
            StmtType first_type, StmtType second_type) const noexcept;
    [[nodiscard]] PairVec<StmtNo> GetFollowsPairsT(
            StmtType first_type, StmtType second_type) const noexcept;

  private:
    friend class ProgramKnowledgeBase;
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
#endif  // SRC_SPA_SRC_PKB_STORE_FOLLOWS_RELATIONSHIP_STORE_H_
