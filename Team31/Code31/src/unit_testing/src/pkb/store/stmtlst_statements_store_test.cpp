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
        REQUIRE(!sss.ExistFollows(true, Index<ArgPos::kFirst>(1),
                                  Index<ArgPos::kSecond>(2)));
        REQUIRE(!sss.ExistFollows(false, Index<ArgPos::kFirst>(1),
                                  Index<ArgPos::kSecond>(10)));
        REQUIRE(sss.ExistFollows(true, Index<ArgPos::kFirst>(1),
                                 Index<ArgPos::kSecond>(10)));
        REQUIRE(sss.ExistFollows(Index<ArgPos::kFirst>(1)));
        REQUIRE(!sss.ExistFollows(Index<ArgPos::kSecond>(5)));
        REQUIRE(sss.ExistFollows(Index<ArgPos::kSecond>(9)));
        REQUIRE(sss.ExistFollows());
    }
    SECTION("GetFollows") {
        REQUIRE(sss.GetFollows(true, Index<ArgPos::kFirst>(1)) ==
                std::vector<int>{3, 4, 10});
        REQUIRE(sss.GetFollows(false, Index<ArgPos::kFirst>(1)) ==
                std::vector<int>{3});
        REQUIRE(sss.GetFollows(true, Index<ArgPos::kSecond>(2)) ==
                std::vector<int>{});
        REQUIRE(sss.GetFollows(false, Index<ArgPos::kSecond>(6)) ==
                std::vector<int>{5});
    }
}
}  // namespace spa