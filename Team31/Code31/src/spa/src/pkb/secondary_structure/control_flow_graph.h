#ifndef SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CONTROL_FLOW_GRAPH_H_
#define SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CONTROL_FLOW_GRAPH_H_

#include <cstddef>
#include <queue>
#include <set>
#include <utility>
#include <vector>

#include "common/aliases.h"
#include "common/bit_array.h"
#include "common/entity_type_enum.h"
#include "container_forest.h"
#include "pkb/store/parent_relationship_store.h"
#include "pkb/store/stmtlst_parent_store.h"
#include "pkb/store/stmtlst_statements_store.h"
#include "pkb/store/type_statements_store.h"

namespace spa {
struct ControlFlowGraph {
  public:
    struct Node {
        std::vector<StmtNo> prev;
        PairOf<StmtNo> next{0, 0};
        StmtNo start;
        StmtNo stop;
        Node() noexcept = delete;
        explicit Node(StmtNo front) noexcept : start(front), stop(front) {}
    };
    struct Stores {
        const StmtlstStatementsStore& stmtlst_statements;
        const TypeStatementsStore& type_statements;
        const StmtlstParentStore& stmtlst_parent;
    };
    ControlFlowGraph(std::size_t stmt_count, std::size_t stmtlst_count,
                     Stores stores) noexcept;
    Node& GetNode(StmtNo stmt_no) noexcept;
    [[nodiscard]] const Node& GetNode(StmtNo stmt_no) const noexcept;

    using NodeIndex = int;
    std::vector<Node> nodes;
    std::vector<NodeIndex> stmt_node_index;
    std::vector<StmtNo> prev;
    std::vector<StmtNo> next;
    std::vector<StmtNo> roots;

  private:
    const StmtlstStatementsStore& stmtlst_stmt_;
    const TypeStatementsStore& type_store_;
    const StmtlstParentStore& stmtlst_parent_;

    void AddIfNode(StmtNo if_stmt, StmtNo prev_stmt = 0) noexcept;
    void AddWhileNode(StmtNo while_stmt, StmtNo prev_stmt = 0) noexcept;
    void ProcessWhileExitPoints(Node& while_node, StmtNo last_stmt,
                                StmtNo while_stmt);
    StmtNo AddCombinedNode(StmtNo first_stmt) noexcept;
    void Precompute() noexcept;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CONTROL_FLOW_GRAPH_H_
