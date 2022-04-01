#include "pkb/store/stmtlst_statements_store.h"

#include "catch.hpp"

namespace spa {
TEST_CASE("pkb/store/StmtlstStatementsStore") {
    StmtlstStatementsStore sss(4, 10);
    sss.Set(1, std::vector<int>{1, 3, 4, 10});
    sss.Set(2, std::vector<int>{2});
    sss.Set(3, std::vector<int>{5, 6});
    sss.Set(4, std::vector<int>{7, 8, 9});
    SECTION("GetStmtlst") { REQUIRE(sss.GetStmtlst(10) == 1); }
    SECTION("GetStmtRelativePos") { REQUIRE(sss.GetStmtRelativePos(10) == 3); }
    SECTION("GetStmtProperties") {
        REQUIRE(sss.GetStmtProperties(10).stmtlst_index == 1);
        REQUIRE(sss.GetStmtProperties(10).pos_in_stmtlst == 3);
    }
    SECTION("GetStatements") {
        REQUIRE(sss.GetStatements(2) == std::vector<int>{2});
    }
    SECTION("ExistFollows") {
        REQUIRE_FALSE(sss.ExistFollowsT(Index<ArgPos::kFirst>(1),
                                        Index<ArgPos::kSecond>(2)));
        REQUIRE_FALSE(sss.ExistFollows(Index<ArgPos::kFirst>(1),
                                       Index<ArgPos::kSecond>(10)));
        REQUIRE(sss.ExistFollowsT(Index<ArgPos::kFirst>(1),
                                  Index<ArgPos::kSecond>(10)));
        REQUIRE(sss.ExistFollows(Index<ArgPos::kFirst>(1)));
        REQUIRE_FALSE(sss.ExistFollows(Index<ArgPos::kSecond>(5)));
        REQUIRE(sss.ExistFollows(Index<ArgPos::kSecond>(9)));
        REQUIRE(sss.ExistFollows());
    }
    SECTION("GetFollows") {
        REQUIRE(sss.GetFollowsTSecondArg(Index<ArgPos::kFirst>(1)) ==
                std::vector<int>{3, 4, 10});
        REQUIRE(sss.GetFollowsSecondArg(Index<ArgPos::kFirst>(1)) == 3);
        REQUIRE(sss.GetFollowsTFirstArg(Index<ArgPos::kSecond>(2)).empty());
        REQUIRE(sss.GetFollowsFirstArg(Index<ArgPos::kSecond>(6)) == 5);
    }
    SECTION("GetFollowsWildcard") {
        REQUIRE(sss.GetFollowsWildcard() ==
                std::vector<int>{3, 4, 10, 6, 8, 9});
    }
    SECTION("GetFollowedByWildcard") {
        REQUIRE(sss.GetFollowedByWildcard() ==
                std::vector<int>{1, 3, 4, 5, 7, 8});
    }
    SECTION("GetFollowsPairs") {
        REQUIRE(sss.GetFollowsPairs().second.size() == 6);
        REQUIRE(sss.GetFollowsPairsT().second.size() == 10);
    }
}
}  // namespace spa
