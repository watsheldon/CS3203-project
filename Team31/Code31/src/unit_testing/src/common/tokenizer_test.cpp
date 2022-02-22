#include "common/tokenizer.h"

#include <string_view>

#include "catch.hpp"

namespace spa {
TEST_CASE("common/Tokenizer") {
    SECTION("next (basic tests)") {
        std::string_view code1{""};
        std::string_view code2{"   hello world!   "};
        std::string_view code3{"\n123abc\t"};

        Tokenizer tokenizer1(code1);
        Tokenizer tokenizer2(code2);
        Tokenizer tokenizer3(code3);

        REQUIRE(tokenizer1.Next() == "");
        REQUIRE(tokenizer1.Next() == "");

        REQUIRE(tokenizer2.Next() == "hello");
        REQUIRE(tokenizer2.Next() == "world");
        REQUIRE(tokenizer2.Next() == "!");
        REQUIRE(tokenizer2.Next() == "");

        REQUIRE(tokenizer3.Next() == "123");
        REQUIRE(tokenizer3.Next() == "abc");
        REQUIRE(tokenizer3.Next() == "");
    }

    SECTION("next (comprehensive tests with whitespaces)") {
        std::string_view code4{
                "procedure sumDigits {\n\tread number;\n\tsum = 0;\nwhile ( ! "
                "\n (\n number > 0 \t) ) { digit = number % 10; while (a == 0 "
                "&& b <= 0) { while (a != b || b >= c) { a = b;\n }\n } sum = "
                "sum + digit; number = number / 10; }call Haha233test1; print "
                "sum;}procedure Haha233test1 {    if (CS3203-A < 6*100 ) then "
                "{    	read read;    } else {print Haha233test1;}}"};
        Tokenizer tokenizer4(code4);

        REQUIRE(tokenizer4.Next() == "procedure");
    }

    SECTION("next (comprehensive tests with whitespaces stripped") {
        std::string_view code5{
                "procedure sumDigits{read "
                "number;sum=0;while(!(number>0)){digit=number%10;while(a==0&&b<"
                "=0){while(a!=b||b>=c){a=b;}}sum=sum+digit;number=number/"
                "10;}call Haha233test1;print sum;}procedure "
                "Haha233test1{if(CS3203-A<6*100)then{read read;}else{print "
                "Haha233test1;}}"};
        Tokenizer tokenizer5(code5);
    }
}
}  // namespace spa
