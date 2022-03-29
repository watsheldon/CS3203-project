#include "common/polish_notation_node.h"

#include "catch.hpp"

namespace spa {
TEST_CASE("common/PolishNotationNode") {
    auto bl = OperatorType::kBracketL;
    auto br = OperatorType::kBracketR;
    auto plus = OperatorType::kPlus;
    auto minus = OperatorType::kMinus;
    auto times = OperatorType::kTimes;
    auto divide = OperatorType::kDivide;
    auto modulo = OperatorType::kModulo;

    SECTION("distinguish between operators with the same precedence") {
        REQUIRE(bl == bl);
        REQUIRE_FALSE(bl == br);
        REQUIRE_FALSE(plus == minus);
        REQUIRE_FALSE(times == divide);
        REQUIRE_FALSE(modulo == times);
        REQUIRE_FALSE(divide == modulo);
    }
    SECTION("operator precedence") {
        REQUIRE(OprHigherEqual(bl, bl));
        REQUIRE(OprHigherEqual(bl, br));
        REQUIRE_FALSE(OprHigherEqual(bl, plus));
        REQUIRE_FALSE(OprHigherEqual(bl, minus));
        REQUIRE_FALSE(OprHigherEqual(bl, times));
        REQUIRE_FALSE(OprHigherEqual(bl, divide));
        REQUIRE_FALSE(OprHigherEqual(bl, modulo));

        REQUIRE(OprHigherEqual(br, bl));
        REQUIRE(OprHigherEqual(br, br));
        REQUIRE_FALSE(OprHigherEqual(br, plus));
        REQUIRE_FALSE(OprHigherEqual(br, minus));
        REQUIRE_FALSE(OprHigherEqual(br, times));
        REQUIRE_FALSE(OprHigherEqual(br, divide));
        REQUIRE_FALSE(OprHigherEqual(br, modulo));

        REQUIRE(OprHigherEqual(plus, bl));
        REQUIRE(OprHigherEqual(plus, br));
        REQUIRE(OprHigherEqual(plus, plus));
        REQUIRE(OprHigherEqual(plus, minus));
        REQUIRE_FALSE(OprHigherEqual(plus, times));
        REQUIRE_FALSE(OprHigherEqual(plus, divide));
        REQUIRE_FALSE(OprHigherEqual(plus, modulo));

        REQUIRE(OprHigherEqual(minus, bl));
        REQUIRE(OprHigherEqual(minus, br));
        REQUIRE(OprHigherEqual(minus, plus));
        REQUIRE(OprHigherEqual(minus, minus));
        REQUIRE_FALSE(OprHigherEqual(minus, times));
        REQUIRE_FALSE(OprHigherEqual(minus, divide));
        REQUIRE_FALSE(OprHigherEqual(minus, modulo));

        REQUIRE(OprHigherEqual(times, bl));
        REQUIRE(OprHigherEqual(times, br));
        REQUIRE(OprHigherEqual(times, plus));
        REQUIRE(OprHigherEqual(times, minus));
        REQUIRE(OprHigherEqual(times, times));
        REQUIRE(OprHigherEqual(times, divide));
        REQUIRE(OprHigherEqual(times, modulo));

        REQUIRE(OprHigherEqual(divide, bl));
        REQUIRE(OprHigherEqual(divide, br));
        REQUIRE(OprHigherEqual(divide, plus));
        REQUIRE(OprHigherEqual(divide, minus));
        REQUIRE(OprHigherEqual(divide, times));
        REQUIRE(OprHigherEqual(divide, divide));
        REQUIRE(OprHigherEqual(divide, modulo));

        REQUIRE(OprHigherEqual(modulo, bl));
        REQUIRE(OprHigherEqual(modulo, br));
        REQUIRE(OprHigherEqual(modulo, plus));
        REQUIRE(OprHigherEqual(modulo, minus));
        REQUIRE(OprHigherEqual(modulo, times));
        REQUIRE(OprHigherEqual(modulo, divide));
        REQUIRE(OprHigherEqual(modulo, modulo));
    }
}
}  // namespace spa
