#ifndef SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CONTROL_FLOW_GRAPH_H_
#define SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CONTROL_FLOW_GRAPH_H_

#include <cstddef>
#include <set>
#include <utility>
#include <vector>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "pkb/store/stmtlst_parent_store.h"
#include "pkb/store/stmtlst_statements_store.h"
#include "pkb/store/type_statements_store.h"

namespace spa {
class ControlFlowGraph {
  public:
    struct Stores {
        const StmtlstStatementsStore& stmtlst_statements;
        const TypeStatementsStore& type_statements;
        const StmtlstParentStore& stmtlst_parent;
    };
    ControlFlowGraph(std::size_t stmt_count, std::size_t stmtlst_count,
                     Stores stores) noexcept;
    [[nodiscard]] bool HasNext() const noexcept;
    [[nodiscard]] bool HasPrev(StmtNo next) const noexcept;
    [[nodiscard]] bool HasNext(StmtNo prev) const noexcept;
    [[nodiscard]] bool IsNext(StmtNo prev, StmtNo next) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetPrev(StmtNo next,
                                           StmtType stmt_type) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetNext(StmtNo prev,
                                           StmtType stmt_type) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetNextSingleArg(
            ArgPos return_pos, StmtType stmt_type) const noexcept;
    [[nodiscard]] PairVec<StmtNo> GetNextPairs(
            StmtType prev_type, StmtType next_type) const noexcept;

  private:
    struct Node {
        std::vector<StmtNo> prev;
        PairOf<StmtNo> next{0, 0};
        StmtNo start;
        StmtNo stop;
        Node() noexcept = delete;
        explicit Node(StmtNo front) noexcept : start(front), stop(front) {}
    };
    using NodeIndex = int;
    std::vector<Node> nodes_;
    std::vector<NodeIndex> stmt_node_index_;
    std::vector<StmtNo> prev_;
    std::vector<StmtNo> next_;
    const StmtlstStatementsStore& stmtlst_stmt_;
    const TypeStatementsStore& type_store_;
    const StmtlstParentStore& stmtlst_parent_;

    void AddIfNode(StmtNo if_stmt, StmtNo prev_stmt = 0) noexcept;
    void AddWhileNode(StmtNo while_stmt, StmtNo prev_stmt = 0) noexcept;
    void ProcessWhileExitPoints(Node& while_node, StmtNo last_stmt,
                                StmtNo while_stmt);
    StmtNo AddCombinedNode(StmtNo first_stmt) noexcept;
    void Precompute() noexcept;
    Node& GetNode(StmtNo stmt_no) noexcept;
    [[nodiscard]] const Node& GetNode(StmtNo stmt_no) const noexcept;
    [[nodiscard]] PairOf<StmtNo> GetNextPair(StmtNo prev) const noexcept;
    [[nodiscard]] std::set<StmtNo> FilterPairResult(
            const PairOf<StmtNo>& pair, StmtType stmt_type) const noexcept;
    [[nodiscard]] std::set<StmtNo> FilterVecResult(
            const std::vector<StmtNo>& stmts, StmtType type) const;
    [[nodiscard]] PairVec<StmtNo> GetNextPairsNoFilter() const noexcept;
    [[nodiscard]] PairVec<StmtNo> GetNextPairsFilterByNext(
            StmtType type) const noexcept;
    [[nodiscard]] PairVec<StmtNo> GetNextPairsFilterByPrev(StmtType type) const;
    [[nodiscard]] PairVec<StmtNo> GetNextPairsFilterBoth(
            StmtType prev_type, StmtType next_type) const;
    void AddIfMatches(StmtType stmt_type, StmtNo stmt_no,
                      std::set<StmtNo>& results) const;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CONTROL_FLOW_GRAPH_H_
