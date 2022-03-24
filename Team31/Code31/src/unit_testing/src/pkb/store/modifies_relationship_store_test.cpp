#include "pkb/store/modifies_relationship_store.h"

#include "catch.hpp"

namespace spa {
TEST_CASE("pkb/store/ModifiesRelationshipStore") {
    ModifiesRelationshipStore mrs(10, 4, 1);
    mrs.Set(1, 1);
    mrs.Set(2, 4);
    mrs.Set(3, 4);
    SECTION("GetStmtNo") {
        REQUIRE(mrs.GetStmtNo(4) == std::vector<int>{2, 3});
    }
    SECTION("GetVarIndex") { REQUIRE(mrs.GetVarIndex(1) == 1); }
}

}  // namespace spa
