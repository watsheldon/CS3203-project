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
    // b/c
    PolishNotation pn5(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 2),
            PolishNotationNode(OperatorType::kDivide),
            PolishNotationNode(ExprNodeType::kVariable, 3)});
    // a/(b/c)
    PolishNotation pn6(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 1),
            PolishNotationNode(OperatorType::kDivide),
            PolishNotationNode(ExprNodeType::kBracketL),
            PolishNotationNode(ExprNodeType::kVariable, 2),
            PolishNotationNode(OperatorType::kDivide),
            PolishNotationNode(ExprNodeType::kVariable, 3),
            PolishNotationNode(ExprNodeType::kBracketR)});
    // a*(b*c)
    PolishNotation pn7(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 1),
            PolishNotationNode(OperatorType::kTimes),
            PolishNotationNode(ExprNodeType::kBracketL),
            PolishNotationNode(ExprNodeType::kVariable, 2),
            PolishNotationNode(OperatorType::kTimes),
            PolishNotationNode(ExprNodeType::kVariable, 3),
            PolishNotationNode(ExprNodeType::kBracketR)});
    // a%(b%c)
    PolishNotation pn8(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 1),
            PolishNotationNode(OperatorType::kModulo),
            PolishNotationNode(ExprNodeType::kBracketL),
            PolishNotationNode(ExprNodeType::kVariable, 2),
            PolishNotationNode(OperatorType::kModulo),
            PolishNotationNode(ExprNodeType::kVariable, 3),
            PolishNotationNode(ExprNodeType::kBracketR)});
    // ((b)/(c))
    PolishNotation pn9(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kBracketL),
            PolishNotationNode(ExprNodeType::kBracketL),
            PolishNotationNode(ExprNodeType::kVariable, 2),
            PolishNotationNode(ExprNodeType::kBracketR),
            PolishNotationNode(OperatorType::kDivide),
            PolishNotationNode(ExprNodeType::kBracketL),
            PolishNotationNode(ExprNodeType::kVariable, 3),
            PolishNotationNode(ExprNodeType::kBracketR),
            PolishNotationNode(ExprNodeType::kBracketR)});
    // a+b
    PolishNotation pn10(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 1),
            PolishNotationNode(OperatorType::kPlus),
            PolishNotationNode(ExprNodeType::kVariable, 2)});
    // b-c
    PolishNotation pn11(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 2),
            PolishNotationNode(OperatorType::kMinus),
            PolishNotationNode(ExprNodeType::kVariable, 3)});
    // a+b-c
    PolishNotation pn12(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 1),
            PolishNotationNode(OperatorType::kPlus),
            PolishNotationNode(ExprNodeType::kVariable, 2),
            PolishNotationNode(OperatorType::kMinus),
            PolishNotationNode(ExprNodeType::kVariable, 3)});
    // a/b*c%d
    PolishNotation pn13(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 1),
            PolishNotationNode(OperatorType::kDivide),
            PolishNotationNode(ExprNodeType::kVariable, 2),
            PolishNotationNode(OperatorType::kTimes),
            PolishNotationNode(ExprNodeType::kVariable, 3),
            PolishNotationNode(OperatorType::kModulo),
            PolishNotationNode(ExprNodeType::kVariable, 4)});
    // a/b*c
    PolishNotation pn14(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 1),
            PolishNotationNode(OperatorType::kDivide),
            PolishNotationNode(ExprNodeType::kVariable, 2),
            PolishNotationNode(OperatorType::kTimes),
            PolishNotationNode(ExprNodeType::kVariable, 3)});
    // b*c%d
    PolishNotation pn15(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 2),
            PolishNotationNode(OperatorType::kTimes),
            PolishNotationNode(ExprNodeType::kVariable, 3),
            PolishNotationNode(OperatorType::kModulo),
            PolishNotationNode(ExprNodeType::kVariable, 4)});
    // a/b
    PolishNotation pn16(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 1),
            PolishNotationNode(OperatorType::kDivide),
            PolishNotationNode(ExprNodeType::kVariable, 2)});
    // b*c
    PolishNotation pn17(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 2),
            PolishNotationNode(OperatorType::kTimes),
            PolishNotationNode(ExprNodeType::kVariable, 3)});
    // c%d
    PolishNotation pn18(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 3),
            PolishNotationNode(OperatorType::kModulo),
            PolishNotationNode(ExprNodeType::kVariable, 4)});

    SECTION("equals operator") { REQUIRE(pn1 == pn2); }
    SECTION("basic contains") {
        REQUIRE_FALSE(pn1.Contains(pn3));
        REQUIRE(pn1.Contains(pn4));
    }
    SECTION("get all variable indices") {
        REQUIRE(pn1.GetAllVarIndices() == std::vector<int>{6, 233});
        REQUIRE(pn3.GetAllVarIndices() == std::vector<int>{6});
        REQUIRE(pn4.GetAllVarIndices() == std::vector<int>{233});
    }
    SECTION("distinguish /*%") {
        REQUIRE(pn6.Contains(pn5));
        REQUIRE_FALSE(pn7.Contains(pn5));
        REQUIRE_FALSE(pn8.Contains(pn5));
    }
    SECTION("extra parenthesis equality") {
        REQUIRE(pn5 == pn9);
        REQUIRE(pn5.Contains(pn9));
        REQUIRE(pn9.Contains(pn5));
    }
    SECTION("consecutive +-") {
        REQUIRE(pn12.Contains(pn12));
        REQUIRE(pn12.Contains(pn10));
        REQUIRE_FALSE(pn12.Contains(pn11));
    }
    SECTION("consecutive /*%") {
        REQUIRE(pn13.Contains(pn13));
        REQUIRE(pn13.Contains(pn14));
        REQUIRE_FALSE(pn13.Contains(pn15));
        REQUIRE(pn13.Contains(pn16));
        REQUIRE_FALSE(pn13.Contains(pn17));
        REQUIRE_FALSE(pn13.Contains(pn18));
        REQUIRE(pn15.Contains(pn17));
        REQUIRE_FALSE(pn15.Contains(pn18));
    }
}
}  // namespace spa
