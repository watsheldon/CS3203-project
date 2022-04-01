#ifndef SRC_SPA_SRC_PKB_STORE_PARENT_RELATIONSHIP_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_PARENT_RELATIONSHIP_STORE_H_

#include <array>
#include <cstddef>
#include <set>
#include <utility>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "common/index.h"
#include "follows_parent_relationship_base.h"
#include "pkb/secondary_structure/container_forest.h"
#include "stmtlst_parent_store.h"
#include "stmtlst_statements_store.h"
#include "type_statements_store.h"

namespace spa {
class ParentRelationshipStore : public FollowsParentRelationshipBase {
  public:
    explicit ParentRelationshipStore(std::size_t stmt_count,
                                     StoreRefs refs) noexcept;

    [[nodiscard]] bool IsNonTransitive(
            Index<ArgPos::kFirst> parent_stmt,
            Index<ArgPos::kSecond> child_stmt) const noexcept final;
    [[nodiscard]] bool IsTransitive(
            Index<ArgPos::kFirst> parent_stmt,
            Index<ArgPos::kSecond> child_stmt) const noexcept final;
    [[nodiscard]] bool HasSecondValues(
            Index<ArgPos::kFirst> parent_stmt) const noexcept final;
    [[nodiscard]] bool HasFirstValues(
            Index<ArgPos::kSecond> child_stmt) const noexcept final;
    [[nodiscard]] bool ExistRelationship() const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetFirstArg(
            StmtType return_type) const noexcept override;
    [[nodiscard]] std::set<StmtNo> GetSecondArg(
            StmtType return_type) const noexcept override;
    [[nodiscard]] std::set<StmtNo> GetSecondArg(
            Index<ArgPos::kFirst> stmt_no,
            StmtType return_type) const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetSecondArgT(
            Index<ArgPos::kFirst> stmt_no,
            StmtType return_type) const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetFirstArg(
            Index<ArgPos::kSecond> stmt_no,
            StmtType return_type) const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetFirstArgT(
            Index<ArgPos::kSecond> stmt_no,
            StmtType return_type) const noexcept final;
    [[nodiscard]] PairVec<StmtNo> GetBothArgs(
            StmtType parent_type, StmtType child_type) const noexcept final;
    [[nodiscard]] PairVec<StmtNo> GetBothArgsT(
            StmtType parent_type, StmtType child_type) const noexcept final;

  private:
    friend class ProgramKnowledgeBase;
    static constexpr std::array<StmtType, 3> kParentTypes{
            StmtType::kAll,  // to indicate both options below
            StmtType::kWhile, StmtType::kIf};

    std::size_t stmt_count_;
    const ContainerForest *container_forest_{};

    [[nodiscard]] StmtNo GetContainerLastStmt(StmtType type,
                                              StmtNo stmt_no) const noexcept;
    [[nodiscard]] static constexpr bool IsParentType(StmtType type) noexcept;
    [[nodiscard]] inline bool IsParent(StmtNo stmt) const noexcept;
    [[nodiscard]] inline bool HasParent(StmtNo stmt) const noexcept;
    void AddAllChildren(int stmtlst_index, std::vector<StmtNo> &children,
                        StmtType child_type) const noexcept;
    void AddAllTransitivePairs(StmtType parent_type, StmtType child_type,
                               PairVec<StmtNo> &results) const noexcept;
    void AddAllTransitiveChildren(std::vector<StmtNo> &children,
                                  StmtType child_type, StmtNo parent_stmt,
                                  StmtNo last_stmt) const;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_PKB_STORE_PARENT_RELATIONSHIP_STORE_H_
