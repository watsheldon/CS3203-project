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
    [[nodiscard]] bool IsNonTransitive(StmtNo parent_stmt,
                                       StmtNo child_stmt) const noexcept final;
    [[nodiscard]] bool IsTransitive(StmtNo parent,
                                    StmtNo child) const noexcept final;
    [[nodiscard]] bool HasSecondValues(StmtNo parent_stmt) const noexcept final;
    [[nodiscard]] bool HasFirstValues(StmtNo child_stmt) const noexcept final;
    [[nodiscard]] bool ExistRelationship() const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetFirstArg(
            StmtType return_type) const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetSecondArg(
            StmtType return_type) const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetSecondArg(
            StmtNo parent, StmtType return_type) const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetSecondArgT(
            StmtNo parent, StmtType return_type) const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetFirstArg(
            StmtNo child, StmtType return_type) const noexcept final;
    [[nodiscard]] std::set<StmtNo> GetFirstArgT(
            StmtNo child, StmtType return_type) const noexcept final;
    [[nodiscard]] PairVec<StmtNo> GetBothArgs(
            StmtType parent_type, StmtType child_type) const noexcept final;
    [[nodiscard]] PairVec<StmtNo> GetBothArgsT(
            StmtType parent_type, StmtType child_type) const noexcept final;
    [[nodiscard]] StmtNo GetContainerLastStmt(StmtType type,
                                              StmtNo stmt_no) const noexcept;

  private:
    friend class ProgramKnowledgeBase;
    static constexpr std::array<StmtType, 3> kParentTypes{
            StmtType::kAll,  // to indicate both options below
            StmtType::kWhile, StmtType::kIf};

    std::size_t stmt_count_;
    const ContainerForest *container_forest_{};

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
