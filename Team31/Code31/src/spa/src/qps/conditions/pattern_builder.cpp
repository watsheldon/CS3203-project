#include "pattern_builder.h"

namespace spa {

std::unique_ptr<ConditionClause> PatternBuilder::build() {
    return std::unique_ptr<ConditionClause>();
}
PatternBuilder::~PatternBuilder() {}
}  // namespace spa