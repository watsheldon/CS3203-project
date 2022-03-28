#include "common/polish_notation_node.h"

#include "catch.hpp"

namespace spa {
TEST_CASE("common/PolishNotationNode") {
    auto bl = PolishNotationNode(ExprNodeType::kOperator,
                                 OperatorType::kBracketL);
    auto br = PolishNotationNode(ExprNodeType::kOperator,
                                 OperatorType::kBracketR);
    auto plus =
            PolishNotationNode(ExprNodeType::kOperator, OperatorType::kPlus);
    auto minus =
            PolishNotationNode(ExprNodeType::kOperator, OperatorType::kMinus);
    auto times =
            PolishNotationNode(ExprNodeType::kOperator, OperatorType::kTimes);
    auto divide =
            PolishNotationNode(ExprNodeType::kOperator, OperatorType::kDivide);
    auto modulo =
            PolishNotationNode(ExprNodeType::kOperator, OperatorType::kModulo);

    SECTION("distinguish between operators with the same precedence") {
        REQUIRE(bl == bl);
        REQUIRE_FALSE(bl == br);
        REQUIRE_FALSE(plus == minus);
        REQUIRE_FALSE(times == divide);
        REQUIRE_FALSE(modulo == times);
        REQUIRE_FALSE(divide == modulo);
    }
    SECTION("operator precedence") {
        REQUIRE(bl.HasHigherPrecedence(bl));
        REQUIRE(bl.HasHigherPrecedence(br));
        REQUIRE_FALSE(bl.HasHigherPrecedence(plus));
        REQUIRE_FALSE(bl.HasHigherPrecedence(minus));
        REQUIRE_FALSE(bl.HasHigherPrecedence(times));
        REQUIRE_FALSE(bl.HasHigherPrecedence(divide));
        REQUIRE_FALSE(bl.HasHigherPrecedence(modulo));

        REQUIRE(br.HasHigherPrecedence(bl));
        REQUIRE(br.HasHigherPrecedence(br));
        REQUIRE_FALSE(br.HasHigherPrecedence(plus));
        REQUIRE_FALSE(br.HasHigherPrecedence(minus));
        REQUIRE_FALSE(br.HasHigherPrecedence(times));
        REQUIRE_FALSE(br.HasHigherPrecedence(divide));
        REQUIRE_FALSE(br.HasHigherPrecedence(modulo));

        REQUIRE(plus.HasHigherPrecedence(bl));
        REQUIRE(plus.HasHigherPrecedence(br));
        REQUIRE(plus.HasHigherPrecedence(plus));
        REQUIRE(plus.HasHigherPrecedence(minus));
        REQUIRE_FALSE(plus.HasHigherPrecedence(times));
        REQUIRE_FALSE(plus.HasHigherPrecedence(divide));
        REQUIRE_FALSE(plus.HasHigherPrecedence(modulo));

        REQUIRE(minus.HasHigherPrecedence(bl));
        REQUIRE(minus.HasHigherPrecedence(br));
        REQUIRE(minus.HasHigherPrecedence(plus));
        REQUIRE(minus.HasHigherPrecedence(minus));
        REQUIRE_FALSE(minus.HasHigherPrecedence(times));
        REQUIRE_FALSE(minus.HasHigherPrecedence(divide));
        REQUIRE_FALSE(minus.HasHigherPrecedence(modulo));

        REQUIRE(times.HasHigherPrecedence(bl));
        REQUIRE(times.HasHigherPrecedence(br));
        REQUIRE(times.HasHigherPrecedence(plus));
        REQUIRE(times.HasHigherPrecedence(minus));
        REQUIRE(times.HasHigherPrecedence(times));
        REQUIRE(times.HasHigherPrecedence(divide));
        REQUIRE(times.HasHigherPrecedence(modulo));

        REQUIRE(divide.HasHigherPrecedence(bl));
        REQUIRE(divide.HasHigherPrecedence(br));
        REQUIRE(divide.HasHigherPrecedence(plus));
        REQUIRE(divide.HasHigherPrecedence(minus));
        REQUIRE(divide.HasHigherPrecedence(times));
        REQUIRE(divide.HasHigherPrecedence(divide));
        REQUIRE(divide.HasHigherPrecedence(modulo));

        REQUIRE(modulo.HasHigherPrecedence(bl));
        REQUIRE(modulo.HasHigherPrecedence(br));
        REQUIRE(modulo.HasHigherPrecedence(plus));
        REQUIRE(modulo.HasHigherPrecedence(minus));
        REQUIRE(modulo.HasHigherPrecedence(times));
        REQUIRE(modulo.HasHigherPrecedence(divide));
        REQUIRE(modulo.HasHigherPrecedence(modulo));
    }
}
}  // namespace spa
