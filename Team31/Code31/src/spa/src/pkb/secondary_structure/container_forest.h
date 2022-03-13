#ifndef SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CONTAINER_FOREST_H_
#define SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CONTAINER_FOREST_H_

#include <cstddef>
#include <vector>

#include "container_node.h"
#include "pkb/store/stmtlst_parent_store.h"
#include "pkb/store/stmtlst_statements_store.h"

namespace spa {
class ContainerForest {
  public:
    ContainerForest(const StmtlstParentStore &stmtlst_parent_store,
                    const StmtlstStatementsStore &stmtlst_stmts_store,
                    std::size_t stmtlst_count) noexcept;
    [[nodiscard]] bool IsParentT(int parent, int child) const noexcept;
    [[nodiscard]] std::vector<int> GetAncestryTrace(int child) const noexcept;
    [[nodiscard]] std::vector<int> GetChildren(int parent) const noexcept;
    /**
     * Retrieves index of the last stmtLst nested within the given stmtLst. If
     * the given stmtLst does not contain any other stmtLst, the given stmtLst
     * index will be returned instead.
     */
    [[nodiscard]] int GetRightmostGrandchild(int grandparent) const noexcept;

  private:
    std::vector<ContainerNode> stmtlsts_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CONTAINER_FOREST_H_
