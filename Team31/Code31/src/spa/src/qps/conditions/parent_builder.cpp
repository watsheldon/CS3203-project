#include "parent_builder.h"

namespace spa {

std::unique_ptr<ConditionClause> ParentBuilder::build() {
    return std::unique_ptr<ConditionClause>();
}
ParentBuilder::~ParentBuilder() {}
}  // namespace spa
