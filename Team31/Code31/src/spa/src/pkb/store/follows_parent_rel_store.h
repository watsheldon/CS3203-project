#ifndef SRC_SPA_SRC_PKB_STORE_FOLLOWS_PARENT_REL_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_FOLLOWS_PARENT_REL_STORE_H_

#include <algorithm>
#include <array>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <utility>

#include "common/bitvec2d.h"
#include "common/entity_type_enum.h"
#include "index_bimap.h"
#include "pkb/secondary_structure/container_forest.h"
#include "stmtlst_parent_store.h"
#include "stmtlst_statements_store.h"
#include "type_statements_store.h"

namespace spa {
class FollowsParentRelStore {
  public:
    struct StoreRefs {
        const TypeStatementsStore &type_stmt;
        const StmtlstParentStore &stmtlst_parent;
        const StmtlstStatementsStore &stmtlst_stmt;
    };
    explicit FollowsParentRelStore(size_t stmt_count, StoreRefs refs) noexcept;
    [[nodiscard]] bool ExistFollows(
            bool transitive, Index<ArgPos::kFirst> first_stmt,
            Index<ArgPos::kSecond> second_stmt) const noexcept;
    [[nodiscard]] bool ExistFollows(
            Index<ArgPos::kFirst> first_stmt) const noexcept;
    [[nodiscard]] bool ExistFollows(
            Index<ArgPos::kSecond> second_stmt) const noexcept;
    [[nodiscard]] bool ExistFollows() const noexcept;

    [[nodiscard]] bool ExistParent(
            bool transitive, Index<ArgPos::kFirst> parent_stmt,
            Index<ArgPos::kSecond> child_stmt) const noexcept;
    [[nodiscard]] bool ExistParent(
            Index<ArgPos::kFirst> parent_stmt) const noexcept;
    [[nodiscard]] bool ExistParent(
            Index<ArgPos::kSecond> child_stmt) const noexcept;
    [[nodiscard]] bool ExistParent() const noexcept;
    [[nodiscard]] std::set<int> GetFollows(ArgPos return_pos,
                                           StmtType return_type) const noexcept;

    [[nodiscard]] std::set<int> GetFollows(bool transitive,
                                           Index<ArgPos::kFirst> first_stmt,
                                           StmtType return_type) const noexcept;

    [[nodiscard]] std::set<int> GetFollows(bool transitive,
                                           Index<ArgPos::kSecond> second_stmt,
                                           StmtType return_type) const noexcept;
    [[nodiscard]] PairVec<int> GetFollowsPairs(
            bool transitive, StmtType first_type,
            StmtType second_type) const noexcept;

    [[nodiscard]] std::set<int> GetParent(ArgPos return_pos,
                                          StmtType return_type) const noexcept;
    [[nodiscard]] std::set<int> GetParent(bool transitive,
                                          Index<ArgPos::kFirst> stmt_no,
                                          StmtType return_type) const noexcept;

    [[nodiscard]] std::set<int> GetParent(bool transitive,
                                          Index<ArgPos::kSecond> stmt_no,
                                          StmtType return_type) const noexcept;
    [[nodiscard]] PairVec<int> GetParentPairs(
            bool transitive, StmtType parent_type,
            StmtType child_type) const noexcept;

  private:
    friend class ProgramKnowledgeBase;
    size_t stmt_count_;
    const TypeStatementsStore &type_stmt_;
    const StmtlstParentStore &stmtlst_parent_;
    const StmtlstStatementsStore &stmtlst_stmt_;
    const ContainerForest *container_forest_{};
    [[nodiscard]] std::set<int> Extract(std::vector<int> results,
                                        StmtType return_type) const noexcept;
    [[nodiscard]] PairVec<int> ExtractPairs(PairVec<int> results,
                                            StmtType first,
                                            StmtType second) const noexcept;
    [[nodiscard]] std::set<int> GetAllParents(
            StmtType return_type) const noexcept;
    [[nodiscard]] std::set<int> GetAllChildren(
            StmtType return_type) const noexcept;
    void FillChildren(int parent, std::vector<int> &results) const noexcept;
    void FillChildrenT(int parent, std::vector<int> &results) const noexcept;
    void FillParents(int child, std::vector<int> &results) const noexcept;
    void FillParentsT(int child, std::vector<int> &results) const noexcept;
    void FillParentTPairs(PairVec<int> &results) const noexcept;
    void FillParentPairs(PairVec<int> &results) const noexcept;
    [[nodiscard]] int GetContainerLastStmt(StmtType type,
                                           int stmt_no) const noexcept;
    static constexpr std::array<StmtType, 3> parent_types_{
            StmtType::kAll, StmtType::kWhile, StmtType::kIf};
    [[nodiscard]] inline bool IsParent(int stmt) const noexcept;
    [[nodiscard]] static inline bool IsParent(StmtType type) noexcept;
    [[nodiscard]] inline bool HasParent(int stmt) const noexcept;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_PKB_STORE_FOLLOWS_PARENT_REL_STORE_H_
