#include "qps/PQL_tokenizer.h"

#include <string_view>

#include "catch.hpp"

namespace spa {
TEST_CASE("common/PQLTokenizer") {
    SECTION("next (basic queries)") {
        std::string_view code1{""};
        std::string_view code2{
                "  procedure p;\n"
                "Select p"};
        std::string_view code3{
                "stmt s;\n"
                "Select s such that Follows* (6, s)"};

        PQLTokenizer tokenizer1;
        tokenizer1(code1);
        PQLTokenizer tokenizer2;
        tokenizer2(code2);
        PQLTokenizer tokenizer3;
        tokenizer3(code3);

        REQUIRE(tokenizer1.Next().empty());
        REQUIRE(tokenizer1.Next().empty());

        REQUIRE(tokenizer2.Next() == "procedure");
        REQUIRE(tokenizer2.Next() == "p");
        REQUIRE(tokenizer2.Next() == ";");
        REQUIRE(tokenizer2.Next() == "Select");
        REQUIRE(tokenizer2.Next() == "p");

        REQUIRE(tokenizer3.Next() == "stmt");
        REQUIRE(tokenizer3.Next() == "s");
        REQUIRE(tokenizer3.Next() == ";");
        REQUIRE(tokenizer3.Next() == "Select");
        REQUIRE(tokenizer3.Next() == "s");
        REQUIRE(tokenizer3.Next() == "such");
        REQUIRE(tokenizer3.Next() == "that");
        REQUIRE(tokenizer3.Next() == "Follows");
        REQUIRE(tokenizer3.Next() == "*");
        REQUIRE(tokenizer3.Next() == "(");
        REQUIRE(tokenizer3.Next() == "6");
        REQUIRE(tokenizer3.Next() == ",");
        REQUIRE(tokenizer3.Next() == "s");
        REQUIRE(tokenizer3.Next() == ")");
    }

    SECTION("next (queries with expression in pattern)") {
        std::string_view code4{
                "assign newa;\n"
                "Select newa pattern newa ( \"normSq\" , _\"cenX * cenX\"_)"};
        PQLTokenizer tokenizer4;
        tokenizer4(code4);
        REQUIRE(tokenizer4.Next() == "assign");
        REQUIRE(tokenizer4.Next() == "newa");
        REQUIRE(tokenizer4.Next() == ";");
        REQUIRE(tokenizer4.Next() == "Select");
        REQUIRE(tokenizer4.Next() == "newa");
        REQUIRE(tokenizer4.Next() == "pattern");
        REQUIRE(tokenizer4.Next() == "newa");
        REQUIRE(tokenizer4.Next() == "(");
        REQUIRE(tokenizer4.Next() == "\"");
        REQUIRE(tokenizer4.Next() == "normSq");
        REQUIRE(tokenizer4.Next() == "\"");
        REQUIRE(tokenizer4.Next() == ",");
        REQUIRE(tokenizer4.Next() == "_");
        REQUIRE(tokenizer4.Next() == "\"");
        REQUIRE(tokenizer4.Next() == "cenX");
        REQUIRE(tokenizer4.Next() == "*");
        REQUIRE(tokenizer4.Next() == "cenX");
        REQUIRE(tokenizer4.Next() == "\"");
        REQUIRE(tokenizer4.Next() == "_");
        REQUIRE(tokenizer4.Next() == ")");
        REQUIRE(tokenizer4.Next().empty());
    }
    SECTION("next (complex queries with diff rels") {
        std::string_view code5{
                "assign a; while w;\n"
                "Select a such that Parent* (w, a)     pattern a(\"x\",     "
                "_\"read % print \"_"};
        PQLTokenizer tokenizer5;
        tokenizer5(code5);
        REQUIRE(tokenizer5.Next() == "assign");
        REQUIRE(tokenizer5.Next() == "a");
        REQUIRE(tokenizer5.Next() == ";");
        REQUIRE(tokenizer5.Next() == "while");
        REQUIRE(tokenizer5.Next() == "w");
        REQUIRE(tokenizer5.Next() == ";");
        REQUIRE(tokenizer5.Next() == "Select");
        REQUIRE(tokenizer5.Next() == "a");
        REQUIRE(tokenizer5.Next() == "such");
        REQUIRE(tokenizer5.Next() == "that");
        REQUIRE(tokenizer5.Next() == "Parent");
        REQUIRE(tokenizer5.Next() == "*");
        REQUIRE(tokenizer5.Next() == "(");
        REQUIRE(tokenizer5.Next() == "w");
        REQUIRE(tokenizer5.Next() == ",");
        REQUIRE(tokenizer5.Next() == "a");
        REQUIRE(tokenizer5.Next() == ")");
        REQUIRE(tokenizer5.Next() == "pattern");
        REQUIRE(tokenizer5.Next() == "a");
        REQUIRE(tokenizer5.Next() == "(");
        REQUIRE(tokenizer5.Next() == "\"");
        REQUIRE(tokenizer5.Next() == "x");
        REQUIRE(tokenizer5.Next() == "\"");
        REQUIRE(tokenizer5.Next() == ",");
        REQUIRE(tokenizer5.Next() == "_");
        REQUIRE(tokenizer5.Next() == "\"");
        REQUIRE(tokenizer5.Next() == "read");
        REQUIRE(tokenizer5.Next() == "%");
        REQUIRE(tokenizer5.Next() == "print");
        REQUIRE(tokenizer5.Next() == "\"");
        REQUIRE(tokenizer5.Next() == "_");
    }
}
}  // namespace spa
