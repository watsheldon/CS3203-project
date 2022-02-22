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

    // dummy pn
    PolishNotation pn0(std::vector<PolishNotationNode>{});
    // v1+1*v3
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

    QueryToken token1 = QueryToken(QueryTokenType::WORD, "v1");
    QueryToken token2 = QueryToken(QueryTokenType::WORD, "v2");
    QueryToken token3 = QueryToken(QueryTokenType::WORD, "v3");

    // _"v1"_
    auto queryToken1 =
            std::vector<QueryToken>{QueryToken(QueryTokenType::WORD, "v1")};
    //_"1"_
    auto queryToken2 =
            std::vector<QueryToken>{QueryToken(QueryTokenType::INTEGER, "1")};

    // _"v1+1"_
    auto queryToken3 =
            std::vector<QueryToken>{QueryToken(QueryTokenType::WORD, "v1"),
                                    QueryToken(QueryTokenType::PLUS),
                                    QueryToken(QueryTokenType::INTEGER, "1")};

    // _"v1+1*v3"_
    auto queryToken4 =
            std::vector<QueryToken>{QueryToken(QueryTokenType::WORD, "v1"),
                                    QueryToken(QueryTokenType::PLUS),
                                    QueryToken(QueryTokenType::INTEGER, "1"),
                                    QueryToken(QueryTokenType::TIMES),
                                    QueryToken(QueryTokenType::WORD, "v3")};

    // _"1*v3"_
    auto queryToken5 =
            std::vector<QueryToken>{QueryToken(QueryTokenType::INTEGER, "1"),
                                    QueryToken(QueryTokenType::TIMES),
                                    QueryToken(QueryTokenType::WORD, "v3")};

    // _"v3*1"_
    auto queryToken6 =
            std::vector<QueryToken>{QueryToken(QueryTokenType::WORD, "v3"),
                                    QueryToken(QueryTokenType::TIMES),
                                    QueryToken(QueryTokenType::INTEGER, "1")};

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

    pkb.SetIndex(Index<SetEntityType::kStmt>(7),
                 Index<SetEntityType::kNotation>(1));
    pkb.SetIndex(Index<SetEntityType::kStmt>(8),
                 Index<SetEntityType::kNotation>(2));
    pkb.SetIndex(Index<SetEntityType::kStmt>(9),
                 Index<SetEntityType::kNotation>(3));
    pkb.SetRel(Index<SetEntityType::kStmt>(7), Index<SetEntityType::kVar>(1));
    pkb.SetRel(Index<SetEntityType::kStmt>(8), Index<SetEntityType::kVar>(2));
    pkb.SetRel(Index<SetEntityType::kStmt>(9), Index<SetEntityType::kVar>(3));

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
    SECTION("GetFollows") {
        REQUIRE(pkb.GetFollows(false, Index<ArgPos::kFirst>(1),
                               StmtType::kRead) == std::set<int>{10});
        REQUIRE(pkb.GetFollows(true, Index<ArgPos::kFirst>(1),
                               StmtType::kAll) == std::set<int>{10});
        REQUIRE(pkb.GetFollows(true, Index<ArgPos::kSecond>(4),
                               StmtType::kAll) == std::set<int>{2, 3});
        REQUIRE(pkb.GetFollows(true, Index<ArgPos::kSecond>(8),
                               StmtType::kWhile) == std::set<int>{5});
    }
    SECTION("GetFollowsWildcard") {
        REQUIRE(pkb.GetFollows(ArgPos::kFirst, StmtType::kAssign).empty());
        REQUIRE(pkb.GetFollows(ArgPos::kSecond, StmtType::kAssign) ==
                std::set<int>{7, 8});
    }
    SECTION("GetFollowsPairs") {
        REQUIRE(pkb.GetFollowsPairs(true, StmtType::kAll, StmtType::kAll)
                        .first.size() == 6);
        REQUIRE(pkb.GetFollowsPairs(false, StmtType::kAll, StmtType::kAll)
                        .first.size() == 5);
    }
    SECTION("GetParent") {
        REQUIRE(pkb.GetParent(true, Index<ArgPos::kFirst>(1),
                              StmtType::kRead) == std::set<int>{2});
        REQUIRE(pkb.GetParent(false, Index<ArgPos::kFirst>(4),
                              StmtType::kAll) == std::set<int>{5,8,9});
        REQUIRE(pkb.GetParent(true, Index<ArgPos::kFirst>(4),
                              StmtType::kAll) == std::set<int>{5,6,7,8,9});
        REQUIRE(pkb.GetParent(false, Index<ArgPos::kSecond>(7),
                              StmtType::kWhile) == std::set<int>{5});
        REQUIRE(pkb.GetParent(true, Index<ArgPos::kSecond>(3),
                              StmtType::kIf).empty());
    }
    SECTION("GetParentWildcard") {
        REQUIRE(pkb.GetParent(ArgPos::kFirst, StmtType::kAssign).size()==0);
        REQUIRE(pkb.GetParent(ArgPos::kFirst, StmtType::kWhile)== std::set<int>{1,5});
        REQUIRE(pkb.GetParent(ArgPos::kSecond, StmtType::kAll) ==
                std::set<int>{2,3,4,5,6,7,8,9});
    }
    SECTION("GetParentPairs") {
        REQUIRE(pkb.GetParentPairs(true, StmtType::kAll, StmtType::kAll)
                        .first.size() == 15);
        REQUIRE(pkb.GetParentPairs(false, StmtType::kAll, StmtType::kAll)
                        .first.size() == 8);
    }

    SECTION("GetPattern") {
        std::pair<std::vector<int>, std::vector<int>> pairTest = {{7}, {1}};
        std::pair<std::vector<int>, std::vector<int>> pairTest2 = {{}, {}};
        std::pair<std::vector<int>, std::vector<int>> pairTest3 = {{7, 8, 9},
                                                                   {1, 2, 3}};
        REQUIRE(pkb.GetPattern(token1) == std::set<int>{7});
        REQUIRE(pkb.GetPattern(token2) == std::set<int>{8});
        REQUIRE(pkb.GetPattern(token3) == std::set<int>{9});
        REQUIRE(pkb.GetPattern(queryToken1, true) == std::set<int>{7});
        REQUIRE(pkb.GetPattern(queryToken2, true) == std::set<int>{7});
        REQUIRE(pkb.GetPattern(queryToken3, true) == std::set<int>{});
        REQUIRE(pkb.GetPattern(queryToken4, true) == std::set<int>{7});
        REQUIRE(pkb.GetPattern(queryToken5, true) == std::set<int>{7});
        REQUIRE(pkb.GetPattern(queryToken6, true) == std::set<int>{});
        REQUIRE(pkb.GetPattern(token1, queryToken4, true) == std::set<int>{7});
        REQUIRE(pkb.GetPattern(token2, queryToken4, true) == std::set<int>{});
        REQUIRE(pkb.GetPatternPair(queryToken4, true) == pairTest);
        REQUIRE(pkb.GetPatternPair(queryToken3, true) == pairTest2);
        REQUIRE(pkb.GetPatternPair() == pairTest3);
    }
}

}  // namespace spa
