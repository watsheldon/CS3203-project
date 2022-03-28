#include "common/polish_notation_node.h"

#include "catch.hpp"

namespace spa {
TEST_CASE("common/PolishNotationNode") {
    auto plus = PolishNotationNode(OperatorType::kPlus);
    auto minus = PolishNotationNode(OperatorType::kMinus);
    auto times = PolishNotationNode(OperatorType::kTimes);
    auto divide = PolishNotationNode(OperatorType::kDivide);
    auto modulo = PolishNotationNode(OperatorType::kModulo);

    SECTION("distinguish between operators with the same precedence") {
        REQUIRE_FALSE(plus == minus);
        REQUIRE_FALSE(times == divide);
        REQUIRE_FALSE(modulo == times);
        REQUIRE_FALSE(divide == modulo);
    }
    SECTION("operator precedence") {
        REQUIRE(plus.HasHigherPrecedence(plus));
        REQUIRE(plus.HasHigherPrecedence(minus));
        REQUIRE_FALSE(plus.HasHigherPrecedence(times));
        REQUIRE_FALSE(plus.HasHigherPrecedence(divide));
        REQUIRE_FALSE(plus.HasHigherPrecedence(modulo));

        REQUIRE(minus.HasHigherPrecedence(plus));
        REQUIRE(minus.HasHigherPrecedence(minus));
        REQUIRE_FALSE(minus.HasHigherPrecedence(times));
        REQUIRE_FALSE(minus.HasHigherPrecedence(divide));
        REQUIRE_FALSE(minus.HasHigherPrecedence(modulo));

        REQUIRE(times.HasHigherPrecedence(plus));
        REQUIRE(times.HasHigherPrecedence(minus));
        REQUIRE(times.HasHigherPrecedence(times));
        REQUIRE(times.HasHigherPrecedence(divide));
        REQUIRE(times.HasHigherPrecedence(modulo));

        REQUIRE(divide.HasHigherPrecedence(plus));
        REQUIRE(divide.HasHigherPrecedence(minus));
        REQUIRE(divide.HasHigherPrecedence(times));
        REQUIRE(divide.HasHigherPrecedence(divide));
        REQUIRE(divide.HasHigherPrecedence(modulo));

        REQUIRE(modulo.HasHigherPrecedence(plus));
        REQUIRE(modulo.HasHigherPrecedence(minus));
        REQUIRE(modulo.HasHigherPrecedence(times));
        REQUIRE(modulo.HasHigherPrecedence(divide));
        REQUIRE(modulo.HasHigherPrecedence(modulo));
    }
}
}  // namespace spa
