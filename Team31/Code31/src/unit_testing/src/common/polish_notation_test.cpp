#include "common/polish_notation.h"

#include <vector>

#include "catch.hpp"
#include "common/polish_notation_node.h"

namespace spa {
TEST_CASE("common/PolishNotation") {
    // v+3*x
    PolishNotation pn1(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 6),
            PolishNotationNode(OperatorType::kPlus),
            PolishNotationNode(ExprNodeType::kConstant, 1),
            PolishNotationNode(OperatorType::kTimes),
            PolishNotationNode(ExprNodeType::kVariable, 233)});
    // v+(3*x)
    PolishNotation pn2(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 6),
            PolishNotationNode(OperatorType::kPlus),
            PolishNotationNode(ExprNodeType::kBracketL),
            PolishNotationNode(ExprNodeType::kConstant, 1),
            PolishNotationNode(OperatorType::kTimes),
            PolishNotationNode(ExprNodeType::kVariable, 233),
            PolishNotationNode(ExprNodeType::kBracketR)});
    // v+3
    PolishNotation pn3(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 6),
            PolishNotationNode(OperatorType::kPlus),
            PolishNotationNode(ExprNodeType::kConstant, 1)});
    // 3*x
    PolishNotation pn4(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kConstant, 1),
            PolishNotationNode(OperatorType::kTimes),
            PolishNotationNode(ExprNodeType::kVariable, 233)});

    REQUIRE(pn1 == pn2);

    SECTION("contains") {
        REQUIRE_FALSE(pn1.Contains(pn3));
        REQUIRE(pn1.Contains(pn4));
    }
    SECTION("get all variable indices") {
        REQUIRE(pn1.GetAllVarIndices() == std::vector<int>{6, 233});
        REQUIRE(pn3.GetAllVarIndices() == std::vector<int>{6});
        REQUIRE(pn4.GetAllVarIndices() == std::vector<int>{233});
    }
}
}  // namespace spa
