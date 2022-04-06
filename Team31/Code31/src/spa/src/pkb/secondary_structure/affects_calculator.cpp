#include "affects_calculator.h"

#include <set>

#include "common/aliases.h"
#include "common/entity_type_enum.h"

namespace spa {
AffectsCalculator::AffectsCalculator() noexcept {}
bool AffectsCalculator::ExistAffects(StmtNo first_assign,
                                     StmtNo second_assign) const noexcept {
    return false;
}
bool AffectsCalculator::ExistAffectsT(StmtNo first_assign,
                                      StmtNo second_assign) const noexcept {
    return false;
}
bool AffectsCalculator::HasAffected(StmtNo first_assign) const noexcept {
    return false;
}
bool AffectsCalculator::HasAffecter(StmtNo second_assign) const noexcept {
    return false;
}
bool AffectsCalculator::ExistAffects() const noexcept { return false; }
std::set<StmtNo> AffectsCalculator::GetAffects(
        ArgPos return_pos) const noexcept {
    return std::set<StmtNo>();
}
std::set<StmtNo> AffectsCalculator::GetAffected(
        StmtNo first_assign) const noexcept {
    return std::set<StmtNo>();
}
std::set<StmtNo> AffectsCalculator::GetAffectedT(
        StmtNo first_assign) const noexcept {
    return std::set<StmtNo>();
}
std::set<StmtNo> AffectsCalculator::GetAffecter(
        StmtNo second_assign) const noexcept {
    return std::set<StmtNo>();
}
std::set<StmtNo> AffectsCalculator::GetAffecterT(
        StmtNo second_assign) const noexcept {
    return std::set<StmtNo>();
}
PairVec<StmtNo> AffectsCalculator::GetAffectsPairs() const noexcept {
    return spa::PairVec<StmtNo>();
}
PairVec<StmtNo> AffectsCalculator::GetAffectsTPairs() const noexcept {
    return spa::PairVec<StmtNo>();
}
}  // namespace spa
