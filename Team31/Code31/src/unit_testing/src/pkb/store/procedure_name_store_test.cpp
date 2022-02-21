#include "pkb/store/procedure_name_store.h"

#include "catch.hpp"

namespace spa {
TEST_CASE("pkb/store/ProcedureNameStore") {
    ProcedureNameStore pns({std::string(), "a", "b", "c"});
    SECTION("GetName") { REQUIRE(pns.GetName(3) == "c"); }
    SECTION("GetIndex") { REQUIRE(pns.GetIndex("c") == 3); }
    SECTION("GetInvalidIndex") { REQUIRE(pns.GetIndex("dd") == 0); }
    std::vector<std::string> values = {std::string(), "a", "b", "c"};
    SECTION("GetAllNames") { REQUIRE(pns.GetAllNames() == values); }
    SECTION("size") { REQUIRE(pns.size() == 3); }
}
}  // namespace spa
