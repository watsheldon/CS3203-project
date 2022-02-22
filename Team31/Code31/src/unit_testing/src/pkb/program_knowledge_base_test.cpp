#include "pkb/program_knowledge_base.h"

#include "catch.hpp"
#include "common/polish_notation_node.h"
#include "pkb/knowledge_base.h"

namespace spa {
TEST_CASE("pkb/ProgramKnowledgeBase") {
    BasicEntities be;
    be.procedures = std::vector<std::string>{"", "p1"};
    be.variables = std::vector<std::string>{"", "v1", "v2", "v3"};
    be.constants = std::vector<std::string>{"", "1", "2", "3"};
    be.reads = std::vector<int>{2, 10};
    be.prints = std::vector<int>{3, 6};
    be.calls = std::vector<int>{};
    be.whiles = std::vector<int>{1, 5};
    be.ifs = std::vector<int>{4};
    be.assigns = std::vector<int>{7, 8, 9};

    // v+1*x
    PolishNotation pn0(std::vector<PolishNotationNode>{});
    // v+1*x
    PolishNotation pn1(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 1),
            PolishNotationNode(OperatorType::kPlus),
            PolishNotationNode(ExprNodeType::kConstant, 1),
            PolishNotationNode(OperatorType::kTimes),
            PolishNotationNode(ExprNodeType::kVariable, 3)});
    // v2+(2*v3)
    PolishNotation pn2(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 2),
            PolishNotationNode(OperatorType::kPlus),
            PolishNotationNode(ExprNodeType::kBracketL),
            PolishNotationNode(ExprNodeType::kConstant, 2),
            PolishNotationNode(OperatorType::kTimes),
            PolishNotationNode(ExprNodeType::kVariable, 3),
            PolishNotationNode(ExprNodeType::kBracketR)});
    // v3+3
    PolishNotation pn3(std::vector<PolishNotationNode>{
            PolishNotationNode(ExprNodeType::kVariable, 3),
            PolishNotationNode(OperatorType::kPlus),
            PolishNotationNode(ExprNodeType::kConstant, 3)});
    be.notations = std::vector<PN>{pn0, pn1, pn2, pn3};

    ProgramKnowledgeBase pkb(be);
    pkb.SetIndex(Index<SetEntityType::kProc>(1),
                 Index<SetEntityType::kStmtLst>(1));
    pkb.SetIndex(Index<SetEntityType::kStmt>(1),
                 Index<SetEntityType::kStmtLst>(2));
    pkb.SetIndex(Index<SetEntityType::kStmt>(5),
                 Index<SetEntityType::kStmtLst>(4));
    pkb.SetIndex(Index<SetEntityType::kStmt>(4),
                 Index<SetEntityType::kStmtLst>(3),
                 Index<SetEntityType::kStmtLst>(5));
    pkb.SetLst(Index<SetEntityType::kStmtLst>(1), std::vector<int>{1, 10});
    pkb.SetLst(Index<SetEntityType::kStmtLst>(2), std::vector<int>{2, 3, 4});
    pkb.SetLst(Index<SetEntityType::kStmtLst>(3), std::vector<int>{5, 8});
    pkb.SetLst(Index<SetEntityType::kStmtLst>(4), std::vector<int>{6, 7});
    pkb.SetLst(Index<SetEntityType::kStmtLst>(5), std::vector<int>{9});
    pkb.Compile();
    SECTION("ExistFollows") {
        REQUIRE(pkb.ExistFollows(false, Index<ArgPos::kFirst>(1),
                                 Index<ArgPos::kSecond>(10)));
        REQUIRE(pkb.ExistFollows(true, Index<ArgPos::kFirst>(2),
                                 Index<ArgPos::kSecond>(4)));
        REQUIRE_FALSE(pkb.ExistFollows(true, Index<ArgPos::kFirst>(5),
                                       Index<ArgPos::kSecond>(9)));
        REQUIRE(pkb.ExistFollows(Index<ArgPos::kFirst>(3)));
        REQUIRE(pkb.ExistFollows(Index<ArgPos::kFirst>(5)));
        REQUIRE_FALSE(pkb.ExistFollows(Index<ArgPos::kSecond>(2)));
        REQUIRE_FALSE(pkb.ExistFollows(Index<ArgPos::kSecond>(9)));
        REQUIRE(pkb.ExistFollows());
    }
    SECTION("ExistParent") {
        REQUIRE(pkb.ExistParent(false, Index<ArgPos::kFirst>(1),
                                Index<ArgPos::kSecond>(3)));
        REQUIRE(pkb.ExistParent(true, Index<ArgPos::kFirst>(4),
                                Index<ArgPos::kSecond>(7)));
        REQUIRE(pkb.ExistParent(false, Index<ArgPos::kFirst>(4),
                                Index<ArgPos::kSecond>(8)));
        REQUIRE_FALSE(pkb.ExistParent(true, Index<ArgPos::kFirst>(2),
                                      Index<ArgPos::kSecond>(8)));
        REQUIRE(pkb.ExistParent(Index<ArgPos::kFirst>(4)));
        REQUIRE_FALSE(pkb.ExistParent(Index<ArgPos::kFirst>(2)));
        REQUIRE(pkb.ExistParent(Index<ArgPos::kSecond>(2)));
        REQUIRE_FALSE(pkb.ExistParent(Index<ArgPos::kSecond>(10)));
        REQUIRE(pkb.ExistParent());
    }
}

}  // namespace spa