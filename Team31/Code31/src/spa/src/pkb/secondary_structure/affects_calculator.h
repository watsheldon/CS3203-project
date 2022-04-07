#ifndef SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_AFFECTS_CALCULATOR_H_
#define SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_AFFECTS_CALCULATOR_H_

#include <set>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "container_forest.h"
#include "control_flow_graph.h"
#include "next_calculator.h"
#include "pkb/store/modifies_relationship_store.h"
#include "pkb/store/stmtlst_statements_store.h"
#include "pkb/store/uses_relationship_store.h"

namespace spa {
class AffectsCalculator {
  public:
    struct Stores {
        const StmtlstStatementsStore& stmtlst_statements;
        const TypeStatementsStore& type_statements;
        const ContainerForest& forest;
        const ModifiesRelationshipStore& modifies_store;
        const UsesRelationshipStore& uses_store;
        const ControlFlowGraph& cfg;
        const NextCalculator& next;
    };
    explicit AffectsCalculator(Stores stores) noexcept;
    [[nodiscard]] bool ExistAffects(StmtNo first_assign,
                                    StmtNo second_assign) const noexcept;
    [[nodiscard]] bool ExistAffectsT(StmtNo first_assign,
                                     StmtNo second_assign) const noexcept;
    [[nodiscard]] bool HasAffected(StmtNo first_assign) const noexcept;
    [[nodiscard]] bool HasAffecter(StmtNo second_assign) const noexcept;
    [[nodiscard]] bool ExistAffects() const noexcept;
    [[nodiscard]] std::set<StmtNo> GetAffects(ArgPos return_pos) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetAffected(
            StmtNo first_assign) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetAffectedT(
            StmtNo first_assign) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetAffecter(
            StmtNo second_assign) const noexcept;
    [[nodiscard]] std::set<StmtNo> GetAffecterT(
            StmtNo second_assign) const noexcept;
    [[nodiscard]] PairVec<StmtNo> GetAffectsPairs() const noexcept;
    [[nodiscard]] PairVec<StmtNo> GetAffectsTPairs() const noexcept;

  private:
    const StmtlstStatementsStore& stmtlst_stmt_;
    const TypeStatementsStore& type_store_;
    const ContainerForest& forest_;
    const ModifiesRelationshipStore& modifies_store_;
    const UsesRelationshipStore& uses_store_;
    const ControlFlowGraph& cfg_;
    const NextCalculator& next_;

    [[nodiscard]] bool IsSameProcedure(StmtNo first_assign,
                                       StmtNo second_assign) const noexcept;
    [[nodiscard]] bool ExistUnmodifiedPath(StmtNo first_assign,
                                           StmtNo second_assign,
                                           VarIndex var) const noexcept;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_AFFECTS_CALCULATOR_H_
