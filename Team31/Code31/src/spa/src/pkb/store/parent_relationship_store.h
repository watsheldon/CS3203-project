#ifndef SRC_SPA_SRC_PKB_STORE_PARENT_RELATIONSHIP_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_PARENT_RELATIONSHIP_STORE_H_

#include <array>
#include <cstddef>
#include <set>
#include <utility>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "common/index.h"
#include "pkb/secondary_structure/container_forest.h"
#include "stmtlst_parent_store.h"
#include "stmtlst_statements_store.h"
#include "type_statements_store.h"

namespace spa {
class ParentRelationshipStore {
  public:
    struct StoreRefs {
        const TypeStatementsStore &type_stmt;
        const StmtlstParentStore &stmtlst_parent;
        const StmtlstStatementsStore &stmtlst_stmt;
    };
    explicit ParentRelationshipStore(std::size_t stmt_count,
                                     StoreRefs refs) noexcept;

    [[nodiscard]] bool ExistParent(
            Index<ArgPos::kFirst> parent_stmt,
            Index<ArgPos::kSecond> child_stmt) const noexcept;
    [[nodiscard]] bool ExistParentT(
            Index<ArgPos::kFirst> parent_stmt,
            Index<ArgPos::kSecond> child_stmt) const noexcept;
    [[nodiscard]] bool ExistParent(
            Index<ArgPos::kFirst> parent_stmt) const noexcept;
    [[nodiscard]] bool ExistParent(
            Index<ArgPos::kSecond> child_stmt) const noexcept;
    [[nodiscard]] bool ExistParent() const noexcept;
    [[nodiscard]] std::set<StmtNo> GetParent(
            ArgPos return_pos, StmtType return_type) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetParent(
            Index<ArgPos::kFirst> stmt_no, StmtType return_type) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetParentT(
            Index<ArgPos::kFirst> stmt_no, StmtType return_type) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetParent(
            Index<ArgPos::kSecond> stmt_no,
            StmtType return_type) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetParentT(
            Index<ArgPos::kSecond> stmt_no,
            StmtType return_type) const noexcept;

    [[nodiscard]] PairVec<StmtNo> GetParentPairs(
            StmtType parent_type, StmtType child_type) const noexcept;
    [[nodiscard]] PairVec<StmtNo> GetParentPairsT(
            StmtType parent_type, StmtType child_type) const noexcept;

  private:
    friend class ProgramKnowledgeBase;
    static constexpr std::array<StmtType, 3> parent_types_{
            StmtType::kAll, StmtType::kWhile, StmtType::kIf};

    std::size_t stmt_count_;
    const TypeStatementsStore &type_stmt_;
    const StmtlstParentStore &stmtlst_parent_;
    const StmtlstStatementsStore &stmtlst_stmt_;
    const ContainerForest *container_forest_{};

    [[nodiscard]] std::set<StmtNo> Extract(std::vector<StmtNo> results,
                                           StmtType return_type) const noexcept;
    [[nodiscard]] PairVec<StmtNo> ExtractPairs(PairVec<StmtNo> results,
                                               StmtType first,
                                               StmtType second) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetAllParents(
            StmtType return_type) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetAllChildren(
            StmtType return_type) const noexcept;
    void FillChildren(StmtNo parent,
                      std::vector<StmtNo> &results) const noexcept;
    void FillChildrenT(StmtNo parent,
                       std::vector<StmtNo> &results) const noexcept;
    void FillParents(StmtNo child, std::vector<StmtNo> &results) const noexcept;
    void FillParentsT(StmtNo child,
                      std::vector<StmtNo> &results) const noexcept;
    void FillParentTPairs(PairVec<StmtNo> &results) const noexcept;
    void FillParentPairs(PairVec<StmtNo> &results) const noexcept;
    [[nodiscard]] StmtNo GetContainerLastStmt(StmtType type,
                                              StmtNo stmt_no) const noexcept;
    [[nodiscard]] inline bool IsParent(StmtNo stmt) const noexcept;
    [[nodiscard]] static inline bool IsParent(StmtType type) noexcept;
    [[nodiscard]] inline bool HasParent(StmtNo stmt) const noexcept;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_PKB_STORE_PARENT_RELATIONSHIP_STORE_H_
