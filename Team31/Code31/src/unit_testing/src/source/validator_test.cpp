#include "source/validator.h"

#include "catch.hpp"
#include "source/token.h"

namespace spa {
TEST_CASE("source/Validator") {
    Token procedure_token = Token(SourceTokenType::kKeywordProcedure);
    Token read_token = Token(SourceTokenType::kKeywordRead);
    Token print_token = Token(SourceTokenType::kKeywordPrint);
    Token call_token = Token(SourceTokenType::kKeywordCall);
    Token while_token = Token(SourceTokenType::kKeywordWhile);
    Token if_token = Token(SourceTokenType::kKeywordIf);
    Token then_token = Token(SourceTokenType::kKeywordThen);
    Token else_token = Token(SourceTokenType::kKeywordElse);
    Token bracketl_token = Token(SourceTokenType::kBracketL);
    Token bracketr_token = Token(SourceTokenType::kBracketR);
    Token bracel_token = Token(SourceTokenType::kBraceL);
    Token bracer_token = Token(SourceTokenType::kBraceR);
    Token assign_equal_token = Token(SourceTokenType::kAssignEqual);
    Token plus_token = Token(SourceTokenType::kOperatorPlus);
    Token minus_token = Token(SourceTokenType::kOperatorMinus);
    Token times_token = Token(SourceTokenType::kOperatorTimes);
    Token divide_token = Token(SourceTokenType::kOperatorDivide);
    Token modulo_token = Token(SourceTokenType::kOperatorModulo);
    Token not_token = Token(SourceTokenType::kCondNot);
    Token and_token = Token(SourceTokenType::kCondAnd);
    Token or_token = Token(SourceTokenType::kCondOr);
    Token lt_token = Token(SourceTokenType::kRelLt);
    Token leq_token = Token(SourceTokenType::kRelLeq);
    Token eq_token = Token(SourceTokenType::kRelEq);
    Token neq_token = Token(SourceTokenType::kRelNeq);
    Token gt_token = Token(SourceTokenType::kRelGt);
    Token geq_token = Token(SourceTokenType::kRelGeq);
    Token semicolon_token = Token(SourceTokenType::kSemicolon);

    SECTION("validate source_1 (comprehensive source code with all token "
            "types)") {
        Token name_token_sumDigits = Token(SourceTokenType::kName, "sumDigits");
        Token name_token_number = Token(SourceTokenType::kName, "number");
        Token name_token_sum = Token(SourceTokenType::kName, "sum");
        Token name_token_digit = Token(SourceTokenType::kName, "digit");
        Token name_token_a = Token(SourceTokenType::kName, "a");
        Token name_token_b = Token(SourceTokenType::kName, "b");
        Token name_token_c = Token(SourceTokenType::kName, "c");
        Token name_token_Haha233test1 =
                Token(SourceTokenType::kName, "Haha233test1");
        Token name_token_CS3203 = Token(SourceTokenType::kName, "CS3203");
        Token name_token_read = Token(SourceTokenType::kName, "read");
        Token int_token_0 = Token(SourceTokenType::kInteger, "0");
        Token int_token_10 = Token(SourceTokenType::kInteger, "10");
        Token int_token_6 = Token(SourceTokenType::kInteger, "6");
        Token int_token_100 = Token(SourceTokenType::kInteger, "100");

        std::vector<Token> tokens1{procedure_token,
                                   name_token_sumDigits,
                                   bracel_token,
                                   read_token,
                                   name_token_number,
                                   semicolon_token,
                                   name_token_sum,
                                   assign_equal_token,
                                   int_token_0,
                                   semicolon_token,
                                   while_token,
                                   bracketl_token,
                                   not_token,
                                   bracketl_token,
                                   name_token_number,
                                   gt_token,
                                   int_token_0,
                                   bracketr_token,
                                   bracketr_token,
                                   bracel_token,
                                   name_token_digit,
                                   assign_equal_token,
                                   name_token_number,
                                   modulo_token,
                                   int_token_10,
                                   semicolon_token,
                                   while_token,
                                   bracketl_token,
                                   bracketl_token,
                                   name_token_a,
                                   eq_token,
                                   int_token_0,
                                   bracketr_token,
                                   and_token,
                                   bracketl_token,
                                   name_token_b,
                                   leq_token,
                                   int_token_0,
                                   bracketr_token,
                                   bracketr_token,
                                   bracel_token,
                                   while_token,
                                   bracketl_token,
                                   bracketl_token,
                                   name_token_a,
                                   neq_token,
                                   name_token_b,
                                   bracketr_token,
                                   or_token,
                                   bracketl_token,
                                   name_token_b,
                                   geq_token,
                                   name_token_c,
                                   bracketr_token,
                                   bracketr_token,
                                   bracel_token,
                                   name_token_a,
                                   assign_equal_token,
                                   name_token_b,
                                   semicolon_token,
                                   bracer_token,
                                   bracer_token,
                                   name_token_sum,
                                   assign_equal_token,
                                   name_token_sum,
                                   plus_token,
                                   name_token_digit,
                                   semicolon_token,
                                   name_token_number,
                                   assign_equal_token,
                                   name_token_number,
                                   divide_token,
                                   int_token_10,
                                   semicolon_token,
                                   bracer_token,
                                   call_token,
                                   name_token_Haha233test1,
                                   semicolon_token,
                                   print_token,
                                   name_token_sum,
                                   semicolon_token,
                                   bracer_token,
                                   procedure_token,
                                   name_token_Haha233test1,
                                   bracel_token,
                                   if_token,
                                   bracketl_token,
                                   name_token_CS3203,
                                   lt_token,
                                   bracketl_token,
                                   int_token_6,
                                   times_token,
                                   int_token_100,
                                   bracketr_token,
                                   bracketr_token,
                                   then_token,
                                   bracel_token,
                                   read_token,
                                   name_token_read,
                                   semicolon_token,
                                   bracer_token,
                                   else_token,
                                   bracel_token,
                                   print_token,
                                   name_token_Haha233test1,
                                   semicolon_token,
                                   bracer_token,
                                   bracer_token};

        std::filesystem::path path1 = "source_1.txt";
        Validator validator1(path1);
        auto tokens = validator1.Validate();
        REQUIRE(tokens == tokens1);
    }

    SECTION("validate source_2 (procedure/var/keyword same name)") {
        Token name_token_while = Token(SourceTokenType::kName, "while");
        Token int_token_0 = Token(SourceTokenType::kInteger, "0");

        std::vector<Token> tokens2{
                procedure_token, name_token_while, bracel_token,
                while_token,     bracketl_token,   not_token,
                bracketl_token,  name_token_while, neq_token,
                int_token_0,     bracketr_token,   bracketr_token,
                bracel_token,    print_token,      name_token_while,
                semicolon_token, bracer_token,     bracer_token};

        std::filesystem::path path2 = "source_2.txt";
        Validator validator2(path2);
        auto tokens = validator2.Validate();
        REQUIRE(tokens == tokens2);
    }
}
}  // namespace spa
