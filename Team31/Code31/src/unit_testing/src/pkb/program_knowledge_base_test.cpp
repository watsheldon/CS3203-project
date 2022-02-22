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

    QueryToken token1 = QueryToken(QueryTokenType::kWord, "v1");
    QueryToken token2 = QueryToken(QueryTokenType::kWord, "v2");
    QueryToken token3 = QueryToken(QueryTokenType::kWord, "v3");

    // _"v1"_
    auto queryToken1 =
            std::vector<QueryToken>{QueryToken(QueryTokenType::kWord, "v1")};
    //_"1"_
    auto queryToken2 =
            std::vector<QueryToken>{QueryToken(QueryTokenType::kInteger, "1")};

    // _"v1+1"_
    auto queryToken3 =
            std::vector<QueryToken>{QueryToken(QueryTokenType::kWord, "v1"),
                                    QueryToken(QueryTokenType::kOperatorPlus),
                                    QueryToken(QueryTokenType::kInteger, "1")};

    // _"v1+1*v3"_
    auto queryToken4 =
            std::vector<QueryToken>{QueryToken(QueryTokenType::kWord, "v1"),
                                    QueryToken(QueryTokenType::kOperatorPlus),
                                    QueryToken(QueryTokenType::kInteger, "1"),
                                    QueryToken(QueryTokenType::kOperatorTimes),
                                    QueryToken(QueryTokenType::kWord, "v3")};

    // _"1*v3"_
    auto queryToken5 =
            std::vector<QueryToken>{QueryToken(QueryTokenType::kInteger, "1"),
                                    QueryToken(QueryTokenType::kOperatorTimes),
                                    QueryToken(QueryTokenType::kWord, "v3")};

    // _"v3*1"_
    auto queryToken6 =
            std::vector<QueryToken>{QueryToken(QueryTokenType::kWord, "v3"),
                                    QueryToken(QueryTokenType::kOperatorTimes),
                                    QueryToken(QueryTokenType::kInteger, "1")};

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
    pkb.SetRel(Index<SetEntityType::kStmt>(2), Index<SetEntityType::kVar>(1));
    pkb.SetRel(Index<SetEntityType::kStmt>(10), Index<SetEntityType::kVar>(2));
    pkb.SetRel(Index<SetEntityType::kStmt>(7), Index<SetEntityType::kVar>(1));
    pkb.SetRel(Index<SetEntityType::kStmt>(8), Index<SetEntityType::kVar>(2));
    pkb.SetRel(Index<SetEntityType::kStmt>(9), Index<SetEntityType::kVar>(3));
    pkb.SetRel(Index<SetEntityType::kStmt>(7), std::vector<int>{1, 3});
    pkb.SetRel(Index<SetEntityType::kStmt>(8), std::vector<int>{2, 3});
    pkb.SetRel(Index<SetEntityType::kStmt>(9), std::vector<int>{3});
    pkb.SetRel(Index<SetEntityType::kStmt>(3), std::vector<int>{3});
    pkb.SetRel(Index<SetEntityType::kStmt>(6), std::vector<int>{2});

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
                              StmtType::kAll) == std::set<int>{5, 8, 9});
        REQUIRE(pkb.GetParent(true, Index<ArgPos::kFirst>(4), StmtType::kAll) ==
                std::set<int>{5, 6, 7, 8, 9});
        REQUIRE(pkb.GetParent(false, Index<ArgPos::kSecond>(7),
                              StmtType::kWhile) == std::set<int>{5});
        REQUIRE(pkb.GetParent(true, Index<ArgPos::kSecond>(3), StmtType::kIf)
                        .empty());
    }
    SECTION("GetParentWildcard") {
        REQUIRE(pkb.GetParent(ArgPos::kFirst, StmtType::kAssign).empty());
        REQUIRE(pkb.GetParent(ArgPos::kFirst, StmtType::kWhile) ==
                std::set<int>{1, 5});
        REQUIRE(pkb.GetParent(ArgPos::kSecond, StmtType::kAll) ==
                std::set<int>{2, 3, 4, 5, 6, 7, 8, 9});
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
        REQUIRE(pkb.GetPattern(1) == std::set<int>{7});
        REQUIRE(pkb.GetPattern(2) == std::set<int>{8});
        REQUIRE(pkb.GetPattern(3) == std::set<int>{9});
        REQUIRE(pkb.GetPattern(queryToken1, true) == std::set<int>{7});
        REQUIRE(pkb.GetPattern(queryToken2, true) == std::set<int>{7});
        REQUIRE(pkb.GetPattern(queryToken3, true).empty());
        REQUIRE(pkb.GetPattern(queryToken4, true) == std::set<int>{7});
        REQUIRE(pkb.GetPattern(queryToken5, true) == std::set<int>{7});
        REQUIRE(pkb.GetPattern(queryToken6, true).empty());
        REQUIRE(pkb.GetPattern(1, queryToken4, true) == std::set<int>{7});
        REQUIRE(pkb.GetPattern(2, queryToken4, true).empty());
        REQUIRE(pkb.GetPatternPair(queryToken4, true) == pairTest);
        REQUIRE(pkb.GetPatternPair(queryToken3, true) == pairTest2);
        REQUIRE(pkb.GetPatternPair() == pairTest3);
    }

    SECTION("Uses") {
        std::pair<std::vector<int>, std::vector<int>> pairTest = {
                {4, 4, 4, 4, 4, 4}, {1, 3, 2, 3, 3, 2}};
        REQUIRE(pkb.ExistUses(7, 1));
        REQUIRE(pkb.ExistUses(7, 3));
        REQUIRE(pkb.ExistUses(8, 2));
        REQUIRE(pkb.ExistUses(5, 1));
        REQUIRE_FALSE(pkb.ExistUses(2, 1));
        REQUIRE_FALSE(pkb.ExistUses(200, 1));
        REQUIRE(pkb.GetUses(Index<QueryEntityType::kStmt>(7)) ==
                std::set<int>{1, 3});
        REQUIRE(pkb.GetUses(Index<QueryEntityType::kStmt>(8)) ==
                std::set<int>{2, 3});
        REQUIRE(pkb.GetUses(Index<QueryEntityType::kStmt>(800)).empty());
        REQUIRE(pkb.GetUses(Index<QueryEntityType::kStmt>(9)) ==
                std::set<int>{3});
        REQUIRE(pkb.GetUses(Index<QueryEntityType::kStmt>(5)) ==
                std::set<int>{1, 2, 3});
        REQUIRE(pkb.GetUses(Index<QueryEntityType::kStmt>(4)) ==
                std::set<int>{1, 2, 3});
        REQUIRE(pkb.GetUses(Index<QueryEntityType::kVar>(1),
                            StmtType::kAssign) == std::set<int>{7});
        REQUIRE(pkb.GetUses(Index<QueryEntityType::kVar>(3),
                            StmtType::kWhile) == std::set<int>{1, 5});
        REQUIRE(pkb.GetUses(StmtType::kAll) ==
                std::set<int>{1, 5, 4, 3, 6, 7, 8, 9});
        REQUIRE(pkb.GetUses(StmtType::kIf) == std::set<int>{4});
        // pair can contain duplicates
        REQUIRE(pkb.GetUsesStmtVar(StmtType::kIf) == pairTest);
    }

    SECTION("ExistModifies") {
        REQUIRE_FALSE(pkb.ExistModifies(7, 2));
        REQUIRE(pkb.ExistModifies(9, 3));
        REQUIRE_FALSE(pkb.ExistModifies(3, 0));
        REQUIRE(pkb.ExistModifies(1, 0));
        REQUIRE(pkb.ExistModifies(4, 1));
        REQUIRE_FALSE(pkb.ExistModifies(700, 2));
    }
    SECTION("GetModifies") {
        REQUIRE(pkb.GetModifies(Index<QueryEntityType::kStmt>(300)).empty());
        REQUIRE(pkb.GetModifies(Index<QueryEntityType::kStmt>(4)) ==
                std::set<int>{1, 2, 3});
        REQUIRE(pkb.GetModifies(Index<QueryEntityType::kStmt>(10)) ==
                std::set<int>{2});
        REQUIRE(pkb.GetModifies(Index<QueryEntityType::kVar>(1),
                                StmtType::kRead) == std::set<int>{2});
        REQUIRE(pkb.GetModifies(Index<QueryEntityType::kVar>(1),
                                StmtType::kPrint)
                        .empty());
        REQUIRE(pkb.GetModifies(Index<QueryEntityType::kVar>(1),
                                StmtType::kAll) ==
                std::set<int>{1, 2, 4, 5, 7});
        REQUIRE(pkb.GetModifies(Index<QueryEntityType::kVar>(3),
                                StmtType::kWhile) == std::set<int>{1});
        REQUIRE(pkb.GetModifies(StmtType::kAll) ==
                std::set<int>{1, 2, 4, 5, 7, 8, 9, 10});
        // pairs can have duplication
        REQUIRE(pkb.GetModifiesStmtVar(StmtType::kAll).second.size() == 13);
    }
    SECTION("IndexToName") {
        std::vector<int> list1 = {1, 2};
        std::list<std::string> names;
        pkb.IndexToName(QueryEntityType::kVar, list1, names);
        REQUIRE(names == std::list<std::string>{"v1", "v2"});
        std::list<std::string> stmts;
        pkb.IndexToName(QueryEntityType::kStmt, list1, stmts);
        REQUIRE(stmts == std::list<std::string>{"1", "2"});
    }
    SECTION("NameToIndex") {
        REQUIRE(pkb.NameToIndex(QueryEntityType::kVar, "a") == 0);
        REQUIRE(pkb.NameToIndex(QueryEntityType::kVar, "v1") == 1);
        REQUIRE(pkb.NameToIndex(QueryEntityType::kProc, "a") == 0);
        REQUIRE(pkb.NameToIndex(QueryEntityType::kProc, "p1") == 1);
    }
    SECTION("GetEntity") {
        REQUIRE(pkb.GetAllEntityIndices(StmtType::kAssign) ==
                std::vector<int>{7, 8, 9});
        REQUIRE(pkb.GetAllEntityIndices(StmtType::kCall) == std::vector<int>{});
        REQUIRE(pkb.GetAllEntityIndices(StmtType::kIf) == std::vector<int>{4});
        REQUIRE(pkb.GetAllEntityIndices(StmtType::kPrint) ==
                std::vector<int>{3, 6});
        REQUIRE(pkb.GetAllEntityIndices(StmtType::kRead) ==
                std::vector<int>{2, 10});
        REQUIRE(pkb.GetAllEntityIndices(StmtType::kWhile) ==
                std::vector<int>{1, 5});
        REQUIRE(pkb.GetAllEntityIndices(QueryEntityType::kConst) ==
                std::vector<int>{1, 2, 3});
        REQUIRE(pkb.GetAllEntityIndices(QueryEntityType::kProc) ==
                std::vector<int>{1});
        REQUIRE(pkb.GetAllEntityIndices(QueryEntityType::kStmt) ==
                std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
        REQUIRE(pkb.GetAllEntityIndices(QueryEntityType::kVar) ==
                std::vector<int>{1, 2, 3});
    }
}

}  // namespace spa
