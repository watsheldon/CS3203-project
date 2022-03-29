#include "common/polish_notation_node.h"

#include "catch.hpp"

namespace spa {
TEST_CASE("common/PolishNotationNode") {
    auto bl = PolishNotationNode(OperatorType::kBracketL);
    auto br = PolishNotationNode(OperatorType::kBracketR);
    auto plus = PolishNotationNode(OperatorType::kPlus);
    auto minus = PolishNotationNode(OperatorType::kMinus);
    auto times = PolishNotationNode(OperatorType::kTimes);
    auto divide = PolishNotationNode(OperatorType::kDivide);
    auto modulo = PolishNotationNode(OperatorType::kModulo);

    SECTION("distinguish between operators with the same precedence") {
        REQUIRE(bl == bl);
        REQUIRE_FALSE(bl == br);
        REQUIRE_FALSE(plus == minus);
        REQUIRE_FALSE(times == divide);
        REQUIRE_FALSE(modulo == times);
        REQUIRE_FALSE(divide == modulo);
    }
    SECTION("operator precedence") {
        REQUIRE(bl.HasHigherEqualPrecedence(bl));
        REQUIRE(bl.HasHigherEqualPrecedence(br));
        REQUIRE_FALSE(bl.HasHigherEqualPrecedence(plus));
        REQUIRE_FALSE(bl.HasHigherEqualPrecedence(minus));
        REQUIRE_FALSE(bl.HasHigherEqualPrecedence(times));
        REQUIRE_FALSE(bl.HasHigherEqualPrecedence(divide));
        REQUIRE_FALSE(bl.HasHigherEqualPrecedence(modulo));

        REQUIRE(br.HasHigherEqualPrecedence(bl));
        REQUIRE(br.HasHigherEqualPrecedence(br));
        REQUIRE_FALSE(br.HasHigherEqualPrecedence(plus));
        REQUIRE_FALSE(br.HasHigherEqualPrecedence(minus));
        REQUIRE_FALSE(br.HasHigherEqualPrecedence(times));
        REQUIRE_FALSE(br.HasHigherEqualPrecedence(divide));
        REQUIRE_FALSE(br.HasHigherEqualPrecedence(modulo));

        REQUIRE(plus.HasHigherEqualPrecedence(bl));
        REQUIRE(plus.HasHigherEqualPrecedence(br));
        REQUIRE(plus.HasHigherEqualPrecedence(plus));
        REQUIRE(plus.HasHigherEqualPrecedence(minus));
        REQUIRE_FALSE(plus.HasHigherEqualPrecedence(times));
        REQUIRE_FALSE(plus.HasHigherEqualPrecedence(divide));
        REQUIRE_FALSE(plus.HasHigherEqualPrecedence(modulo));

        REQUIRE(minus.HasHigherEqualPrecedence(bl));
        REQUIRE(minus.HasHigherEqualPrecedence(br));
        REQUIRE(minus.HasHigherEqualPrecedence(plus));
        REQUIRE(minus.HasHigherEqualPrecedence(minus));
        REQUIRE_FALSE(minus.HasHigherEqualPrecedence(times));
        REQUIRE_FALSE(minus.HasHigherEqualPrecedence(divide));
        REQUIRE_FALSE(minus.HasHigherEqualPrecedence(modulo));

        REQUIRE(times.HasHigherEqualPrecedence(bl));
        REQUIRE(times.HasHigherEqualPrecedence(br));
        REQUIRE(times.HasHigherEqualPrecedence(plus));
        REQUIRE(times.HasHigherEqualPrecedence(minus));
        REQUIRE(times.HasHigherEqualPrecedence(times));
        REQUIRE(times.HasHigherEqualPrecedence(divide));
        REQUIRE(times.HasHigherEqualPrecedence(modulo));

        REQUIRE(divide.HasHigherEqualPrecedence(bl));
        REQUIRE(divide.HasHigherEqualPrecedence(br));
        REQUIRE(divide.HasHigherEqualPrecedence(plus));
        REQUIRE(divide.HasHigherEqualPrecedence(minus));
        REQUIRE(divide.HasHigherEqualPrecedence(times));
        REQUIRE(divide.HasHigherEqualPrecedence(divide));
        REQUIRE(divide.HasHigherEqualPrecedence(modulo));

        REQUIRE(modulo.HasHigherEqualPrecedence(bl));
        REQUIRE(modulo.HasHigherEqualPrecedence(br));
        REQUIRE(modulo.HasHigherEqualPrecedence(plus));
        REQUIRE(modulo.HasHigherEqualPrecedence(minus));
        REQUIRE(modulo.HasHigherEqualPrecedence(times));
        REQUIRE(modulo.HasHigherEqualPrecedence(divide));
        REQUIRE(modulo.HasHigherEqualPrecedence(modulo));
    }
}
}  // namespace spa
