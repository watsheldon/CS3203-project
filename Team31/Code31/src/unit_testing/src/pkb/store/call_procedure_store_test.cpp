#include "pkb/store/call_procedure_store.h"

#include "catch.hpp"

namespace spa {
TEST_CASE("pkb/store/CallProcedureStore") {
    CallProcedureStore cps(10, 2);
    cps.Set(1, 1);
    cps.Set(5, 1);
    cps.Set(10, 2);
    SECTION("GetProcIndex") { REQUIRE(cps.GetProcIndex(10) == 2); }
    SECTION("GetCalls") { REQUIRE(cps.GetCalls(1) == std::vector<int>{1, 5}); }
}

}  // namespace spa
