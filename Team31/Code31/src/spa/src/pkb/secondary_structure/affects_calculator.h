#ifndef SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_AFFECTS_CALCULATOR_H_
#define SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_AFFECTS_CALCULATOR_H_

#include <set>

#include "common/aliases.h"
#include "common/entity_type_enum.h"

namespace spa {
class AffectsCalculator {
  public:
    explicit AffectsCalculator() noexcept;
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
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_AFFECTS_CALCULATOR_H_
