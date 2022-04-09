#include "condition_clause.h"

namespace spa {
ConditionClause::~ConditionClause() = default;
bool ConditionClause::operator<(const ConditionClause& other) const noexcept {
    if (GetSynCount() < other.GetSynCount()) {
        return true;
    }
    if (GetSynCount() == other.GetSynCount()) {
        return GetPriority() < other.GetPriority();
    }
    return false;
}
}  // namespace spa
