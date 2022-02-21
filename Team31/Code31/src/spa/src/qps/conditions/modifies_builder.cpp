#include "modifies_builder.h"

namespace spa {

std::unique_ptr<ConditionClause> ModifiesBuilder::build() {
    return std::unique_ptr<ConditionClause>();
}
ModifiesBuilder::~ModifiesBuilder() {}
}  // namespace spa
