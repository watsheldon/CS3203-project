#include "pkb/store/stmtlst_parent_store.h"

#include "catch.hpp"

namespace spa {
TEST_CASE("pkb/store/StmtlstParentStore") {
    StmtlstParentStore sps(2, 8, 5);
    sps.Set(Index<SetEntityType::kProc>(1), Index<SetEntityType::kStmtLst>(1));
    sps.Set(Index<SetEntityType::kProc>(2), Index<SetEntityType::kStmtLst>(3));
    sps.Set(Index<SetEntityType::kStmt>(2), Index<SetEntityType::kStmtLst>(2));
    sps.Set(Index<SetEntityType::kStmt>(5), Index<SetEntityType::kStmtLst>(4),
            Index<SetEntityType::kStmtLst>(5));

    SECTION("GetParentProc") {
        REQUIRE(sps.GetParent(3).type == StmtlstParentStore::ParentType::kProc);
        REQUIRE(sps.GetParent(3).index == 2);
    }
    SECTION("GetParentWhile") {
        REQUIRE(sps.GetParent(2).type ==
                StmtlstParentStore::ParentType::kWhile);
        REQUIRE(sps.GetParent(2).index == 2);
    }
    SECTION("GetParentIf") {
        REQUIRE(sps.GetParent(4).type == StmtlstParentStore::ParentType::kIf);
        REQUIRE(sps.GetParent(4).index == 5);
        REQUIRE(sps.GetParent(5).type == StmtlstParentStore::ParentType::kIf);
        REQUIRE(sps.GetParent(5).index == 5);
    }
    SECTION("GetProcStmtLst") { REQUIRE(sps.GetProcStmtLst(2) == 3); }
    SECTION("GetWhileStmtLst") { REQUIRE(sps.GetWhileStmtLst(2) == 2); }
    SECTION("GetIfStmtLst") {
        REQUIRE(sps.GetIfStmtLst(5).then_index == 4);
        REQUIRE(sps.GetIfStmtLst(5).else_index == 5);
    }
}
}  // namespace spa
