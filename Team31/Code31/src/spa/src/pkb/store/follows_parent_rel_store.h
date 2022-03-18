#ifndef SRC_SPA_SRC_PKB_STORE_FOLLOWS_PARENT_REL_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_FOLLOWS_PARENT_REL_STORE_H_

#include <algorithm>
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
    explicit FollowsParentRelStore(size_t stmt_count);
    void Compile(std::unique_ptr<TypeStatementsStore> type_stmt,
                 std::unique_ptr<ContainerForest> container_forest,
                 std::unique_ptr<StmtlstParentStore> stmtlst_parent,
                 std::unique_ptr<StmtlstStatementsStore> stmtlst_stmt);
    bool ExistFollows(bool transitive, Index<ArgPos::kFirst> first_stmt,
                      Index<ArgPos::kSecond> second_stmt);
    bool ExistFollows(Index<ArgPos::kFirst> first_stmt);
    bool ExistFollows(Index<ArgPos::kSecond> second_stmt);
    bool ExistFollows();

    bool ExistParent(bool transitive, Index<ArgPos::kFirst> parent_stmt,
                     Index<ArgPos::kSecond> child_stmt);
    bool ExistParent(Index<ArgPos::kFirst> parent_stmt);
    bool ExistParent(Index<ArgPos::kSecond> child_stmt);
    bool ExistParent();
    std::set<int> GetFollows(ArgPos return_pos, StmtType return_type);

    std::set<int> GetFollows(bool transitive, Index<ArgPos::kFirst> first_stmt,
                             StmtType return_type);

    std::set<int> GetFollows(bool transitive,
                             Index<ArgPos::kSecond> second_stmt,
                             StmtType return_type);
    PairVec<int> GetFollowsPairs(bool transitive, StmtType first_type,
                                 StmtType second_type);

    std::set<int> GetParent(ArgPos return_pos, StmtType return_type);
    std::set<int> GetParent(bool transitive, Index<ArgPos::kFirst> stmt_no,
                            StmtType return_type);

    std::set<int> GetParent(bool transitive, Index<ArgPos::kSecond> stmt_no,
                            StmtType return_type);
    PairVec<int> GetParentPairs(bool transitive, StmtType parent_type,
                                StmtType child_type);

  private:
    size_t stmt_count_;
    std::unique_ptr<TypeStatementsStore> type_stmt_;
    std::unique_ptr<ContainerForest> container_forest_;
    std::unique_ptr<StmtlstParentStore> stmtlst_parent_;
    std::unique_ptr<StmtlstStatementsStore> stmtlst_stmt_;
    std::set<int> filter(std::vector<int> results, StmtType return_type);
    std::set<int> GetAllParents(StmtType return_type);
    std::set<int> GetAllChildren(StmtType return_type);
    void GetNonTransitiveParentFirst(StmtType parent_type, int parent,
                                     std::vector<int> &results) const;
    void GetTransitiveParentFirst(StmtType parent_type, int parent,
                                  std::vector<int> &results);
    void GetTransitiveParentPairs(PairVec<int> &results);
    void GetNonTransitiveParentPairs(PairVec<int> &results);
    int GetContainerLastStmt(StmtType type, int stmt_no);
};
}  // namespace spa
#endif  // SRC_SPA_SRC_PKB_STORE_FOLLOWS_PARENT_REL_STORE_H_
