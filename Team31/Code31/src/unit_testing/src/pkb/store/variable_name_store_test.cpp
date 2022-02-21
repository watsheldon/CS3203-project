#include "pkb/store/variable_name_store.h"

#include "catch.hpp"

namespace spa {
TEST_CASE("pkb/store/VariableNameStore") {
    VariableNameStore vns({std::string(), "a", "b", "c"});
    SECTION("GetName") { REQUIRE(vns.GetName(3) == "c"); }
    SECTION("GetIndex") { REQUIRE(vns.GetIndex("c") == 3); }
    SECTION("GetInvalidIndex") { REQUIRE(vns.GetIndex("dd") == 0); }
    std::vector<std::string> values = {std::string(), "a", "b", "c"};
    SECTION("GetAllNames") { REQUIRE(vns.GetAllNames() == values); }
    SECTION("size") { REQUIRE(vns.size() == 3); }
}
}  // namespace spa
