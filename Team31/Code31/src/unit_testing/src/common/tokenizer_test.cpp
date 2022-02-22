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

    SECTION("next (while statement with negate condition)") {
        std::string_view code6{"procedure while{while(!(while!=0)){print while;}}"};
        Tokenizer tokenizer6(code6);
        REQUIRE(tokenizer6.Next() == "procedure");
        REQUIRE(tokenizer6.Next() == "while");
        REQUIRE(tokenizer6.Next() == "{");
        REQUIRE(tokenizer6.Next() == "while");
        REQUIRE(tokenizer6.Next() == "(");
        REQUIRE(tokenizer6.Next() == "!");
        REQUIRE(tokenizer6.Next() == "(");
        REQUIRE(tokenizer6.Next() == "while");
        REQUIRE(tokenizer6.Next() == "!=");
        REQUIRE(tokenizer6.Next() == "0");
        REQUIRE(tokenizer6.Next() == ")");
        REQUIRE(tokenizer6.Next() == ")");
        REQUIRE(tokenizer6.Next() == "{");
        REQUIRE(tokenizer6.Next() == "print");
        REQUIRE(tokenizer6.Next() == "while");
        REQUIRE(tokenizer6.Next() == ";");
        REQUIRE(tokenizer6.Next() == "}");
        REQUIRE(tokenizer6.Next() == "}");
        REQUIRE(tokenizer6.Next() == "");
    }

    SECTION("next (comprehensive tests with whitespaces)") {
        std::string_view code4{
                "procedure sumDigits {\n\tread number;\n\tsum = 0;\nwhile ( ! "
                "\n (\n\n\n number > 0 \t) ) \n\n{ digit = number % 10; while (a == 0 "
                "&& b <= 0) { while (a != b || b >= c) { a = b;\n\n }\n } sum = "
                "sum + digit; number = number / 10; }call Haha233test1; print "
                "sum;}procedure Haha233test1 {    if (CS3203-A < 6*100 ) then "
                "{    	read read;    } else {print Haha233test1;}}"};
        Tokenizer tokenizer4(code4);

        REQUIRE(tokenizer4.Next() == "procedure");
        REQUIRE(tokenizer4.Next() == "sumDigits");
        REQUIRE(tokenizer4.Next() == "{");
        REQUIRE(tokenizer4.Next() == "read");
        REQUIRE(tokenizer4.Next() == "number");
        REQUIRE(tokenizer4.Next() == ";");
        REQUIRE(tokenizer4.Next() == "sum");
        REQUIRE(tokenizer4.Next() == "=");
        REQUIRE(tokenizer4.Next() == "0");
        REQUIRE(tokenizer4.Next() == ";");
        REQUIRE(tokenizer4.Next() == "while");
        REQUIRE(tokenizer4.Next() == "(");
        REQUIRE(tokenizer4.Next() == "!");
        REQUIRE(tokenizer4.Next() == "(");
        REQUIRE(tokenizer4.Next() == "number");
        REQUIRE(tokenizer4.Next() == ">");
        REQUIRE(tokenizer4.Next() == "0");
        REQUIRE(tokenizer4.Next() == ")");
        REQUIRE(tokenizer4.Next() == ")");
        REQUIRE(tokenizer4.Next() == "{");
        REQUIRE(tokenizer4.Next() == "digit");
        REQUIRE(tokenizer4.Next() == "=");
        REQUIRE(tokenizer4.Next() == "number");
        REQUIRE(tokenizer4.Next() == "%");
        REQUIRE(tokenizer4.Next() == "10");
        REQUIRE(tokenizer4.Next() == ";");
        REQUIRE(tokenizer4.Next() == "while");
        REQUIRE(tokenizer4.Next() == "(");
        REQUIRE(tokenizer4.Next() == "a");
        REQUIRE(tokenizer4.Next() == "==");
        REQUIRE(tokenizer4.Next() == "0");
        REQUIRE(tokenizer4.Next() == "&&");
        REQUIRE(tokenizer4.Next() == "b");
        REQUIRE(tokenizer4.Next() == "<=");
        REQUIRE(tokenizer4.Next() == "0");
        REQUIRE(tokenizer4.Next() == ")");
        REQUIRE(tokenizer4.Next() == "{");
        REQUIRE(tokenizer4.Next() == "while");
        REQUIRE(tokenizer4.Next() == "(");
        REQUIRE(tokenizer4.Next() == "a");
        REQUIRE(tokenizer4.Next() == "!=");
        REQUIRE(tokenizer4.Next() == "b");
        REQUIRE(tokenizer4.Next() == "||");
        REQUIRE(tokenizer4.Next() == "b");
        REQUIRE(tokenizer4.Next() == ">=");
        REQUIRE(tokenizer4.Next() == "c");
        REQUIRE(tokenizer4.Next() == ")");
        REQUIRE(tokenizer4.Next() == "{");
        REQUIRE(tokenizer4.Next() == "a");
        REQUIRE(tokenizer4.Next() == "=");
        REQUIRE(tokenizer4.Next() == "b");
        REQUIRE(tokenizer4.Next() == ";");
        REQUIRE(tokenizer4.Next() == "}");
        REQUIRE(tokenizer4.Next() == "}");
        REQUIRE(tokenizer4.Next() == "sum");
        REQUIRE(tokenizer4.Next() == "=");
        REQUIRE(tokenizer4.Next() == "sum");
        REQUIRE(tokenizer4.Next() == "+");
        REQUIRE(tokenizer4.Next() == "digit");
        REQUIRE(tokenizer4.Next() == ";");
        REQUIRE(tokenizer4.Next() == "number");
        REQUIRE(tokenizer4.Next() == "=");
        REQUIRE(tokenizer4.Next() == "number");
        REQUIRE(tokenizer4.Next() == "/");
        REQUIRE(tokenizer4.Next() == "10");
        REQUIRE(tokenizer4.Next() == ";");
        REQUIRE(tokenizer4.Next() == "}");
        REQUIRE(tokenizer4.Next() == "call");
        REQUIRE(tokenizer4.Next() == "Haha233test1");
        REQUIRE(tokenizer4.Next() == ";");
        REQUIRE(tokenizer4.Next() == "print");
        REQUIRE(tokenizer4.Next() == "sum");
        REQUIRE(tokenizer4.Next() == ";");
        REQUIRE(tokenizer4.Next() == "}");
        REQUIRE(tokenizer4.Next() == "procedure");
        REQUIRE(tokenizer4.Next() == "Haha233test1");
        REQUIRE(tokenizer4.Next() == "{");
        REQUIRE(tokenizer4.Next() == "if");
        REQUIRE(tokenizer4.Next() == "(");
        REQUIRE(tokenizer4.Next() == "CS3203");
        REQUIRE(tokenizer4.Next() == "-");
        REQUIRE(tokenizer4.Next() == "A");
        REQUIRE(tokenizer4.Next() == "<");
        REQUIRE(tokenizer4.Next() == "6");
        REQUIRE(tokenizer4.Next() == "*");
        REQUIRE(tokenizer4.Next() == "100");
        REQUIRE(tokenizer4.Next() == ")");
        REQUIRE(tokenizer4.Next() == "then");
        REQUIRE(tokenizer4.Next() == "{");
        REQUIRE(tokenizer4.Next() == "read");
        REQUIRE(tokenizer4.Next() == "read");
        REQUIRE(tokenizer4.Next() == ";");
        REQUIRE(tokenizer4.Next() == "}");
        REQUIRE(tokenizer4.Next() == "else");
        REQUIRE(tokenizer4.Next() == "{");
        REQUIRE(tokenizer4.Next() == "print");
        REQUIRE(tokenizer4.Next() == "Haha233test1");
        REQUIRE(tokenizer4.Next() == ";");
        REQUIRE(tokenizer4.Next() == "}");
        REQUIRE(tokenizer4.Next() == "}");
        REQUIRE(tokenizer4.Next() == "");
    }

    SECTION("next (comprehensive tests with whitespaces stripped)") {
        std::string_view code5{
                "procedure sumDigits{read "
                "number;sum=0;while(!(number>0)){digit=number%10;while(a==0&&b<"
                "=0){while(a!=b||b>=c){a=b;}}sum=sum+digit;number=number/"
                "10;}call Haha233test1;print sum;}procedure "
                "Haha233test1{if(CS3203-A<6*100)then{read read;}else{print "
                "Haha233test1;}}"};
        Tokenizer tokenizer5(code5);

        REQUIRE(tokenizer5.Next() == "procedure");
        REQUIRE(tokenizer5.Next() == "sumDigits");
        REQUIRE(tokenizer5.Next() == "{");
        REQUIRE(tokenizer5.Next() == "read");
        REQUIRE(tokenizer5.Next() == "number");
        REQUIRE(tokenizer5.Next() == ";");
        REQUIRE(tokenizer5.Next() == "sum");
        REQUIRE(tokenizer5.Next() == "=");
        REQUIRE(tokenizer5.Next() == "0");
        REQUIRE(tokenizer5.Next() == ";");
        REQUIRE(tokenizer5.Next() == "while");
        REQUIRE(tokenizer5.Next() == "(");
        REQUIRE(tokenizer5.Next() == "!");
        REQUIRE(tokenizer5.Next() == "(");
        REQUIRE(tokenizer5.Next() == "number");
        REQUIRE(tokenizer5.Next() == ">");
        REQUIRE(tokenizer5.Next() == "0");
        REQUIRE(tokenizer5.Next() == ")");
        REQUIRE(tokenizer5.Next() == ")");
        REQUIRE(tokenizer5.Next() == "{");
        REQUIRE(tokenizer5.Next() == "digit");
        REQUIRE(tokenizer5.Next() == "=");
        REQUIRE(tokenizer5.Next() == "number");
        REQUIRE(tokenizer5.Next() == "%");
        REQUIRE(tokenizer5.Next() == "10");
        REQUIRE(tokenizer5.Next() == ";");
        REQUIRE(tokenizer5.Next() == "while");
        REQUIRE(tokenizer5.Next() == "(");
        REQUIRE(tokenizer5.Next() == "a");
        REQUIRE(tokenizer5.Next() == "==");
        REQUIRE(tokenizer5.Next() == "0");
        REQUIRE(tokenizer5.Next() == "&&");
        REQUIRE(tokenizer5.Next() == "b");
        REQUIRE(tokenizer5.Next() == "<=");
        REQUIRE(tokenizer5.Next() == "0");
        REQUIRE(tokenizer5.Next() == ")");
        REQUIRE(tokenizer5.Next() == "{");
        REQUIRE(tokenizer5.Next() == "while");
        REQUIRE(tokenizer5.Next() == "(");
        REQUIRE(tokenizer5.Next() == "a");
        REQUIRE(tokenizer5.Next() == "!=");
        REQUIRE(tokenizer5.Next() == "b");
        REQUIRE(tokenizer5.Next() == "||");
        REQUIRE(tokenizer5.Next() == "b");
        REQUIRE(tokenizer5.Next() == ">=");
        REQUIRE(tokenizer5.Next() == "c");
        REQUIRE(tokenizer5.Next() == ")");
        REQUIRE(tokenizer5.Next() == "{");
        REQUIRE(tokenizer5.Next() == "a");
        REQUIRE(tokenizer5.Next() == "=");
        REQUIRE(tokenizer5.Next() == "b");
        REQUIRE(tokenizer5.Next() == ";");
        REQUIRE(tokenizer5.Next() == "}");
        REQUIRE(tokenizer5.Next() == "}");
        REQUIRE(tokenizer5.Next() == "sum");
        REQUIRE(tokenizer5.Next() == "=");
        REQUIRE(tokenizer5.Next() == "sum");
        REQUIRE(tokenizer5.Next() == "+");
        REQUIRE(tokenizer5.Next() == "digit");
        REQUIRE(tokenizer5.Next() == ";");
        REQUIRE(tokenizer5.Next() == "number");
        REQUIRE(tokenizer5.Next() == "=");
        REQUIRE(tokenizer5.Next() == "number");
        REQUIRE(tokenizer5.Next() == "/");
        REQUIRE(tokenizer5.Next() == "10");
        REQUIRE(tokenizer5.Next() == ";");
        REQUIRE(tokenizer5.Next() == "}");
        REQUIRE(tokenizer5.Next() == "call");
        REQUIRE(tokenizer5.Next() == "Haha233test1");
        REQUIRE(tokenizer5.Next() == ";");
        REQUIRE(tokenizer5.Next() == "print");
        REQUIRE(tokenizer5.Next() == "sum");
        REQUIRE(tokenizer5.Next() == ";");
        REQUIRE(tokenizer5.Next() == "}");
        REQUIRE(tokenizer5.Next() == "procedure");
        REQUIRE(tokenizer5.Next() == "Haha233test1");
        REQUIRE(tokenizer5.Next() == "{");
        REQUIRE(tokenizer5.Next() == "if");
        REQUIRE(tokenizer5.Next() == "(");
        REQUIRE(tokenizer5.Next() == "CS3203");
        REQUIRE(tokenizer5.Next() == "-");
        REQUIRE(tokenizer5.Next() == "A");
        REQUIRE(tokenizer5.Next() == "<");
        REQUIRE(tokenizer5.Next() == "6");
        REQUIRE(tokenizer5.Next() == "*");
        REQUIRE(tokenizer5.Next() == "100");
        REQUIRE(tokenizer5.Next() == ")");
        REQUIRE(tokenizer5.Next() == "then");
        REQUIRE(tokenizer5.Next() == "{");
        REQUIRE(tokenizer5.Next() == "read");
        REQUIRE(tokenizer5.Next() == "read");
        REQUIRE(tokenizer5.Next() == ";");
        REQUIRE(tokenizer5.Next() == "}");
        REQUIRE(tokenizer5.Next() == "else");
        REQUIRE(tokenizer5.Next() == "{");
        REQUIRE(tokenizer5.Next() == "print");
        REQUIRE(tokenizer5.Next() == "Haha233test1");
        REQUIRE(tokenizer5.Next() == ";");
        REQUIRE(tokenizer5.Next() == "}");
        REQUIRE(tokenizer5.Next() == "}");
        REQUIRE(tokenizer5.Next() == "");
    }
}
}  // namespace spa
