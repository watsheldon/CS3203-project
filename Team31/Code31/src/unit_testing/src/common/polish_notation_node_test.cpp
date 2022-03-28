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
        REQUIRE(plus >= plus);
        REQUIRE(plus >= minus);
        REQUIRE_FALSE(plus >= times);
        REQUIRE_FALSE(plus >= divide);
        REQUIRE_FALSE(plus >= modulo);

        REQUIRE(minus >= plus);
        REQUIRE(minus >= minus);
        REQUIRE_FALSE(minus >= times);
        REQUIRE_FALSE(minus >= divide);
        REQUIRE_FALSE(minus >= modulo);

        REQUIRE(times >= plus);
        REQUIRE(times >= minus);
        REQUIRE(times >= times);
        REQUIRE(times >= divide);
        REQUIRE(times >= modulo);

        REQUIRE(divide >= plus);
        REQUIRE(divide >= minus);
        REQUIRE(divide >= times);
        REQUIRE(divide >= divide);
        REQUIRE(divide >= modulo);

        REQUIRE(modulo >= plus);
        REQUIRE(modulo >= minus);
        REQUIRE(modulo >= times);
        REQUIRE(modulo >= divide);
        REQUIRE(modulo >= modulo);
    }
}
}  // namespace spa
