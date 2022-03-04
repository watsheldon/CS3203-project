#include "pkb/store/name_value_store.h"

#include "catch.hpp"

namespace spa {
TEST_CASE("pkb/store/NameValueStore") {
    NameValueStore cvs({std::string(), "1", "2", "3"});
    NameValueStore vns({std::string(), "a", "b", "c"});
    SECTION("GetValue") {
        REQUIRE(cvs.GetNameValue(3) == "3");
        REQUIRE(vns.GetNameValue(3) == "c");
    }
    SECTION("GetIndex") {
        REQUIRE(cvs.GetIndex("3") == 3);
        REQUIRE(vns.GetIndex("c") == 3);
    }
    SECTION("GetInvalidIndex") {
        REQUIRE(cvs.GetIndex("6") == 0);
        REQUIRE(vns.GetIndex("dd") == 0);
    }
    std::vector<std::string> values1 = {std::string(), "1", "2", "3"};
    std::vector<std::string> values2 = {std::string(), "a", "b", "c"};
    SECTION("GetAllValues") {
        REQUIRE(cvs.GetAllNamesValues() == values1);
        REQUIRE(vns.GetAllNamesValues() == values2);
    }
    SECTION("size") {
        REQUIRE(cvs.size() == 3);
        REQUIRE(vns.size() == 3);
    }
}
}  // namespace spa
