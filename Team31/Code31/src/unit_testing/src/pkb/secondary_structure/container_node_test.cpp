#include "pkb/secondary_structure/container_node.h"

#include "catch.hpp"

namespace spa {
TEST_CASE("pkb/secondary_structure/ContainerNode") {
    ContainerNode cn = ContainerNode();
    cn.SetParent(1);
    cn.SetFirstChild(2);
    cn.SetNextSibling(3);
    SECTION("GetParent") { REQUIRE(cn.GetParent() == 1); }
    SECTION("GetFirstChild") { REQUIRE(cn.GetFirstChild() == 2); }
    SECTION("GetNextSibling") { REQUIRE(cn.GetNextSibling() == 3); }
}

}  // namespace spa
