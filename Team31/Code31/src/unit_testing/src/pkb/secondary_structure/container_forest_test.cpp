#include "pkb/secondary_structure/container_forest.h"

#include "catch.hpp"
#include "pkb/store/stmtlst_parent_store.h"
#include "pkb/store/stmtlst_statements_store.h"

namespace spa {
TEST_CASE("pkb/secondary_structure/ContainerForest") {
    StmtlstStatementsStore sss(5, 10);
    sss.Set(1, std::vector<int>{1, 10});
    sss.Set(2, std::vector<int>{2, 3, 4});
    sss.Set(3, std::vector<int>{5, 8});
    sss.Set(4, std::vector<int>{6, 7});
    sss.Set(5, std::vector<int>{9});
    StmtlstParentStore sps(1, 10, 5);
    sps.Set(Index<SetEntityType::kProc>(1), Index<SetEntityType::kStmtLst>(1));
    sps.Set(Index<SetEntityType::kStmt>(1), Index<SetEntityType::kStmtLst>(2));
    sps.Set(Index<SetEntityType::kStmt>(5), Index<SetEntityType::kStmtLst>(4));
    sps.Set(Index<SetEntityType::kStmt>(4), Index<SetEntityType::kStmtLst>(3),
            Index<SetEntityType::kStmtLst>(5));
    ContainerForest cf(sps, sss, 5);
    SECTION("IsParentT") {
        REQUIRE(cf.IsParentT(3, 4));
        REQUIRE_FALSE(cf.IsParentT(3, 5));
    }
    SECTION("GetAncestryTrace") {
        REQUIRE(cf.GetAncestryTrace(1) == std::vector<int>{1});
        REQUIRE(cf.GetAncestryTrace(5) == std::vector<int>{5, 2, 1});
    }
    SECTION("GetChildren") {
        REQUIRE(cf.GetChildren(5).empty());
        REQUIRE(cf.GetChildren(3) == std::vector<int>{4});
    }
    SECTION("GetRightmostGrandchild") {
        REQUIRE(cf.GetRightmostGrandchild(1) == 5);
    }
}

}  // namespace spa
