#include "pkb/store/type_statements_store.h"

#include "catch.hpp"

namespace spa {
TEST_CASE("pkb/store/TypeStatementsStore") {
    TypeStatementsStore tss(10, std::vector<int>{1, 10}, std::vector<int>{4, 5},
                            std::vector<int>{3}, std::vector<int>{2},
                            std::vector<int>{6}, std::vector<int>{7, 8, 9});
    SECTION("GetStatements") {
        REQUIRE(tss.GetStatements(StmtType::kIf) == std::vector<int>{6});
    }
    SECTION("GetType") { REQUIRE(tss.GetType(10) == StmtType::kRead); }
}

}  // namespace spa
