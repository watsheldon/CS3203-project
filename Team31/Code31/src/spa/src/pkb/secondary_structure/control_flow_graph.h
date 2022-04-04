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
class ControlFlowGraph {
  public:
    struct Stores {
        const StmtlstStatementsStore& stmtlst_statements;
        const TypeStatementsStore& type_statements;
        const StmtlstParentStore& stmtlst_parent;
        const ContainerForest& forest;
        const ParentRelationshipStore& parent_store;
    };
    ControlFlowGraph(std::size_t stmt_count, std::size_t stmtlst_count,
                     Stores stores) noexcept;
    [[nodiscard]] bool HasNext() const noexcept;
    [[nodiscard]] bool HasPrev(StmtNo next) const noexcept;
    [[nodiscard]] bool HasNext(StmtNo prev) const noexcept;
    [[nodiscard]] bool IsNext(StmtNo prev, StmtNo next) const noexcept;
    [[nodiscard]] bool IsNextT(StmtNo prev, StmtNo next) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetPrev(StmtNo next,
                                           StmtType stmt_type) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetPrevT(StmtNo next,
                                            StmtType stmt_type) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetNext(StmtNo prev,
                                           StmtType stmt_type) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetNextT(StmtNo prev,
                                            StmtType stmt_type) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetNextSingleArg(
            ArgPos return_pos, StmtType stmt_type) const noexcept;
    [[nodiscard]] PairVec<StmtNo> GetNextPairs(
            StmtType prev_type, StmtType next_type) const noexcept;
    [[nodiscard]] PairVec<StmtNo> GetNextTPairs(StmtType prev_type,
                                                StmtType next_type) noexcept;
    [[nodiscard]] std::set<StmtNo> GetNextTSelf(
            StmtType stmt_type) const noexcept;

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
    std::vector<StmtNo> roots_;
    std::queue<std::vector<StmtNo>> queue_;
    const StmtlstStatementsStore& stmtlst_stmt_;
    const TypeStatementsStore& type_store_;
    const StmtlstParentStore& stmtlst_parent_;
    const ContainerForest& forest_;
    const ParentRelationshipStore& parent_store_;

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
    [[nodiscard]] std::vector<StmtNo> GetNextTVec(
            StmtNo prev, StmtType stmt_type) const noexcept;
    [[nodiscard]] PairVec<StmtNo> GetNextTPairsNoFilter() noexcept;
    void AddIfMatches(StmtType stmt_type, StmtNo stmt_no,
                      std::set<StmtNo>& results) const;
    [[nodiscard]] PairVec<StmtNo> GetNextTPairsFilterByNext(
            StmtType type) noexcept;
    [[nodiscard]] PairVec<StmtNo> GetNextTPairsFilterByPrev(
            StmtType type) noexcept;
    [[nodiscard]] PairVec<StmtNo> GetNextTPairsFilterBoth(
            StmtType prev_type, StmtType next_type) noexcept;
    void AddTPairsInProc(StmtNo root, PairVec<StmtNo>& results) noexcept;
    [[nodiscard]] std::size_t ToBit(std::size_t row,
                                    std::size_t col) const noexcept;
    void AddPairsWithPrevInQueue(
            PairVec<StmtNo>& results, BitArray& added, StmtNo next_stmt,
            StmtType filter_type = StmtType::kAll) noexcept;
    void AppendAndQueue(PairVec<StmtNo>& results, BitArray& added,
                        StmtNo next_stmt,
                        StmtType type = StmtType::kAll) noexcept;
    void AddTPairsInProcFilterNext(StmtNo root, PairVec<StmtNo>& results,
                                   StmtType next_type) noexcept;
    void AddConsecutiveTPairsFilterNext(PairVec<StmtNo>& results,
                                        BitArray& added, StmtNo start,
                                        StmtNo stop,
                                        StmtType next_type) noexcept;
    void AddTPairsInProcFilterPrev(StmtNo root, PairVec<StmtNo>& results,
                                   StmtType prev_type) noexcept;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CONTROL_FLOW_GRAPH_H_
