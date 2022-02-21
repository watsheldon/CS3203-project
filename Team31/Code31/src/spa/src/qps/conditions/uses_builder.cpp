#include "uses_builder.h"

namespace spa {

std::unique_ptr<ConditionClause> UsesBuilder::build() {
    return std::unique_ptr<ConditionClause>();
}
UsesBuilder::~UsesBuilder() {}
}  // namespace spa
