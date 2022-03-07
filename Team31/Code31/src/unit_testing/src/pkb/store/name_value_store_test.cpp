#include "pkb/store/name_value_store.h"

#include "catch.hpp"

namespace spa {
TEST_CASE("pkb/store/NameValueStore") {
    NameValueStore nvs({std::string(), "f", "g", "h"},
                       {std::string(), "a", "b", "c"},
                       {std::string(), "1", "2", "3"});
    SECTION("GetValue") {
        REQUIRE(nvs.GetNameValue(1, QueryEntityType::kProc) == "f");
        REQUIRE(nvs.GetNameValue(2, QueryEntityType::kProc) == "g");
        REQUIRE(nvs.GetNameValue(3, QueryEntityType::kProc) == "h");
        REQUIRE(nvs.GetNameValue(1, QueryEntityType::kVar) == "a");
        REQUIRE(nvs.GetNameValue(2, QueryEntityType::kVar) == "b");
        REQUIRE(nvs.GetNameValue(3, QueryEntityType::kVar) == "c");
        REQUIRE(nvs.GetNameValue(1, QueryEntityType::kConst) == "1");
        REQUIRE(nvs.GetNameValue(2, QueryEntityType::kConst) == "2");
        REQUIRE(nvs.GetNameValue(3, QueryEntityType::kConst) == "3");
    }
    SECTION("GetIndex") {
        REQUIRE(nvs.GetIndex("f", QueryEntityType::kProc) == 1);
        REQUIRE(nvs.GetIndex("g", QueryEntityType::kProc) == 2);
        REQUIRE(nvs.GetIndex("h", QueryEntityType::kProc) == 3);
        REQUIRE(nvs.GetIndex("a", QueryEntityType::kVar) == 1);
        REQUIRE(nvs.GetIndex("b", QueryEntityType::kVar) == 2);
        REQUIRE(nvs.GetIndex("c", QueryEntityType::kVar) == 3);
        REQUIRE(nvs.GetIndex("1", QueryEntityType::kConst) == 1);
        REQUIRE(nvs.GetIndex("2", QueryEntityType::kConst) == 2);
        REQUIRE(nvs.GetIndex("3", QueryEntityType::kConst) == 3);
    }
    SECTION("GetInvalidIndex") {
        REQUIRE(nvs.GetIndex("func", QueryEntityType::kProc) == 0);
        REQUIRE(nvs.GetIndex("dd", QueryEntityType::kVar) == 0);
        REQUIRE(nvs.GetIndex("6", QueryEntityType::kConst) == 0);
    }
    std::vector<std::string> values1 = {std::string(), "f", "g", "h"};
    std::vector<std::string> values2 = {std::string(), "a", "b", "c"};
    std::vector<std::string> values3 = {std::string(), "1", "2", "3"};
    SECTION("GetAllValues") {
        REQUIRE(nvs.GetAllNamesValues(QueryEntityType::kProc) == values1);
        REQUIRE(nvs.GetAllNamesValues(QueryEntityType::kVar) == values2);
        REQUIRE(nvs.GetAllNamesValues(QueryEntityType::kConst) == values3);
    }
    SECTION("GetSize") {
        REQUIRE(nvs.GetSize(QueryEntityType::kProc) == 3);
        REQUIRE(nvs.GetSize(QueryEntityType::kVar) == 3);
        REQUIRE(nvs.GetSize(QueryEntityType::kConst) == 3);
    }
}
}  // namespace spa
