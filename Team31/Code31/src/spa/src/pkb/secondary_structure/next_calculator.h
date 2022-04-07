#ifndef SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_NEXT_CALCULATOR_H_
#define SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_NEXT_CALCULATOR_H_

#include <set>

#include "common/aliases.h"
#include "common/bitvec2d.h"
#include "common/entity_type_enum.h"
#include "container_forest.h"
#include "control_flow_graph.h"
#include "pkb/store/parent_relationship_store.h"
#include "pkb/store/stmtlst_parent_store.h"
#include "pkb/store/stmtlst_statements_store.h"
#include "pkb/store/type_statements_store.h"

namespace spa {
class NextCalculator {
  public:
    struct Stores {
        const StmtlstStatementsStore& stmtlst_statements;
        const TypeStatementsStore& type_statements;
        const ContainerForest& forest;
        const ParentRelationshipStore& parent_store;
        const ControlFlowGraph& cfg;
    };
    explicit NextCalculator(Stores stores) noexcept;
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
    const StmtlstStatementsStore& stmtlst_stmt_;
    const TypeStatementsStore& type_store_;
    const ContainerForest& forest_;
    const ParentRelationshipStore& parent_store_;
    const ControlFlowGraph& cfg_;

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
    [[nodiscard]] std::vector<StmtNo> GetPrevTVec(
            StmtNo next, StmtType stmt_type) const noexcept;
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
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_NEXT_CALCULATOR_H_
