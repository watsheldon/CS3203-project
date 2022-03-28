#include "pkb/store/polish_notation_store.h"

#include "catch.hpp"
#include "common/polish_notation.h"
#include "common/polish_notation_node.h"

namespace spa {
TEST_CASE("pkb/store/PolishNotation") {
    std::vector<PolishNotation> notations;
    // v+3*x
    PolishNotation pn1(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 6),
            PolishNotationNode(ExprNodeType::kOperator, OperatorType::kPlus),
            PolishNotationNode(ExprNodeType::kConstant, 1),
            PolishNotationNode(ExprNodeType::kOperator, OperatorType::kTimes),
            PolishNotationNode(ExprNodeType::kVariable, 233)});
    notations.emplace_back(pn1);
    notations.emplace_back(pn1);
    // v+(3*x)
    PolishNotation pn2(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 6),
            PolishNotationNode(ExprNodeType::kOperator, OperatorType::kPlus),
            PolishNotationNode(ExprNodeType::kOperator,
                               OperatorType::kBracketL),
            PolishNotationNode(ExprNodeType::kConstant, 1),
            PolishNotationNode(ExprNodeType::kOperator, OperatorType::kTimes),
            PolishNotationNode(ExprNodeType::kVariable, 233),
            PolishNotationNode(ExprNodeType::kOperator,
                               OperatorType::kBracketR)});
    notations.emplace_back(pn2);
    // v+3
    PolishNotation pn3(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 6),
            PolishNotationNode(ExprNodeType::kOperator, OperatorType::kPlus),
            PolishNotationNode(ExprNodeType::kConstant, 1)});
    notations.emplace_back(pn3);
    // 3*x
    PolishNotation pn4(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kConstant, 1),
            PolishNotationNode(ExprNodeType::kOperator, OperatorType::kTimes),
            PolishNotationNode(ExprNodeType::kVariable, 233)});
    notations.emplace_back(pn4);

    NameValueStore nvs({std::string(), "f", "g", "h"},
                       {std::string(), "a", "b", "c"},
                       {std::string(), "1", "2", "3"});

    PolishNotationStore pns(10, std::move(notations), nvs);
    pns.Set(1, 1);
    pns.Set(2, 3);
    pns.Set(10, 2);
    pns.Set(5, 4);
    SECTION("GetNotation") { REQUIRE(pns.GetNotation(4) == pn4); }
    SECTION("GetPolishStmt") { REQUIRE(pns.GetPolishStmt(1) == 1); }
    SECTION("GetPolishIndex") { REQUIRE(pns.GetPolishIndex(10) == 2); }
}

}  // namespace spa
