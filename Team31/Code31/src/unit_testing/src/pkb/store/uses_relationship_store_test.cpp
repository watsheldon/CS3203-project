#include "pkb/store/uses_relationship_store.h"

#include "catch.hpp"

namespace spa {
TEST_CASE("pkb/store/UsesRelationshipStore") {
    UsesRelationshipStore urs(10, 4);
    urs.Set(1, {1, 4});
    urs.Set(2, {4, 2});
    urs.Set(3, {3, 4});
    SECTION("GetStmtNo") {
        REQUIRE(urs.GetStmtNo(4) == std::vector<int>{1, 2, 3});
    }
    SECTION("GetVarIndex") {
        REQUIRE(urs.GetVarIndex(2) == std::vector<int>{4, 2});
    }
}

}  // namespace spa
