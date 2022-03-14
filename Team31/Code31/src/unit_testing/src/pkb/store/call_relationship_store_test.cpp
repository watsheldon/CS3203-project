#include "catch.hpp"
#include "pkb/store/calls_relationship_store.h"

namespace spa {
TEST_CASE("pkb/store/CallRelationshipStore") {
    std::vector<std::set<int>> procs;
    procs.emplace_back(std::set<int>{});
    procs.emplace_back(std::set<int>{2, 3});
    procs.emplace_back(std::set<int>{3, 4});
    procs.emplace_back(std::set<int>{});
    procs.emplace_back(std::set<int>{});
    CallsRelationshipStore crs(15, 4, std::move(procs));
    crs.Set(1, 2);
    crs.Set(5, 3);
    crs.Set(10, 3);
    crs.Set(11, 4);
    SECTION("GetCallerProcs") {
        REQUIRE(crs.GetCallerProcs(3) == std::set<int>{1, 2});
        REQUIRE(crs.GetCallerProcs(1).empty());
    }
    SECTION("GetCalleeProcs") {
        REQUIRE(crs.GetCalleeProcs(1) == std::set<int>{2, 3});
        REQUIRE(crs.GetCalleeProcs(3).empty());
    }
    SECTION("GetCallerProcsT") {
        REQUIRE(crs.GetCallerProcsT(4) == std::set<int>{1, 2});
        REQUIRE(crs.GetCallerProcs(1).empty());
    }
    SECTION("GetCalleeProcsT") {
        REQUIRE(crs.GetCalleeProcsT(1) == std::set<int>{2, 3, 4});
        REQUIRE(crs.GetCalleeProcsT(3).empty());
    }
    SECTION("GetAll") {
        REQUIRE(crs.GetAllCallers() == std::set<int>{1, 2});
        REQUIRE(crs.GetAllCallees() == std::set<int>{2, 3, 4});
    }
    SECTION("GetPairs") {
        REQUIRE(crs.GetCallsPairs().first.size() == 4);
        REQUIRE(crs.GetCallsTPairs().first.size() == 5);
    }
    SECTION("GetStmts") {
        REQUIRE(crs.GetStmts(1).empty());
        REQUIRE(crs.GetStmts(3) == std::vector<int>{5, 10});
    }
    SECTION("GetProc") {
        REQUIRE(crs.GetProc(10) == 3);
        REQUIRE(crs.GetProc(1) == 2);
    }
}

}  // namespace spa
