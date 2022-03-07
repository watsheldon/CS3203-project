#include "pkb/store/calls_relationship_store.h"

#include "catch.hpp"

namespace spa {
TEST_CASE("pkb/store/CallRelationshipStore") {
    std::vector<std::set<int>> procs;
    procs.emplace_back(std::set<int>{});
    procs.emplace_back(std::set<int>{2, 3});
    procs.emplace_back(std::set<int>{3});
    procs.emplace_back(std::set<int>{});
    CallsRelationshipStore crs(15, std::move(procs));
    crs.Set(1, 2);
    crs.Set(5, 3);
    crs.Set(10, 3);
    SECTION("GetCallerProcs") {
        REQUIRE(crs.GetCallerProcs(3) == std::vector<int>{1, 2});
        REQUIRE(crs.GetCallerProcs(1).empty());
    }
    SECTION("GetCalleeProcs") {
        REQUIRE(crs.GetCalleeProcs(1) == std::vector<int>{2, 3});
        REQUIRE(crs.GetCalleeProcs(3).empty());
    }
    SECTION("GetProc") {
        REQUIRE(crs.GetProc(10) == 3);
        REQUIRE(crs.GetProc(1) == 2);
    }
}

}  // namespace spa
