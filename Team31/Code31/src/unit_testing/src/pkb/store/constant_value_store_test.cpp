#include "catch.hpp"

#include "pkb/store/constant_value_store.h"

using namespace spa;

TEST_CASE("pkb/store/ConstantValueStore") {
    ConstantValueStore cvs({std::string(), "1", "2", "3"});
    SECTION("GetValue") { REQUIRE(cvs.GetValue(3) == "3"); }
    SECTION("GetIndex") { REQUIRE(cvs.GetIndex("3") == 3); }
    SECTION("GetInvalidIndex") { REQUIRE(cvs.GetIndex("6") == 0); }
    std::vector<std::string> values = {std::string(), "1", "2", "3"};
    SECTION("GetAllValues") { REQUIRE(cvs.GetAllValues() == values); }
    SECTION("size") { REQUIRE(cvs.size() == 3); }
}
