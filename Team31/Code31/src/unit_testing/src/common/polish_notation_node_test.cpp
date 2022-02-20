#include "common/polish_notation_node.h"

#include "catch.hpp"

namespace spa {
TEST_CASE("common/PolishNotationNode") {
    REQUIRE(PolishNotationNode(OperatorType::kTimes) ==
            PolishNotationNode(ExprNodeType::kOperator, 2));
}
}  // namespace spa