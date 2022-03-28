#include "qps/PQL_validator.h"

#include <string_view>
#include <vector>

#include "catch.hpp"

namespace spa {
TEST_CASE("common/PQLValidator") {
    QueryToken decl_assign = QueryToken(QueryTokenType::kDeclAssign);
    QueryToken decl_proc = QueryToken(QueryTokenType::kDeclProcedure);
    QueryToken decl_while = QueryToken(QueryTokenType::kDeclWhile);
    QueryToken decl_if = QueryToken(QueryTokenType::kDeclIf);
    QueryToken comma = QueryToken(QueryTokenType::kComma);
    QueryToken semicolon = QueryToken(QueryTokenType::kSemicolon);
    QueryToken decl_variable = QueryToken(QueryTokenType::kDeclVariable);
    QueryToken select = QueryToken(QueryTokenType::kKeywordSelect);
    QueryToken pattern = QueryToken(QueryTokenType::kKeywordPattern);
    QueryToken dot = QueryToken(QueryTokenType::kDot);
    QueryToken attr_proc = QueryToken(QueryTokenType::kAttrProc);
    QueryToken such = QueryToken(QueryTokenType::kKeywordSuch);
    QueryToken that = QueryToken(QueryTokenType::kKeywordThat);
    QueryToken calls = QueryToken(QueryTokenType::kKeywordCalls);
    QueryToken parent = QueryToken(QueryTokenType::kKeywordParent);
    QueryToken next = QueryToken(QueryTokenType::kKeywordNext);
    QueryToken bracket_l = QueryToken(QueryTokenType::kBracketL);
    QueryToken bracket_r = QueryToken(QueryTokenType::kBracketR);
    QueryToken quote = QueryToken(QueryTokenType::kQuote);
    QueryToken wildcard = QueryToken(QueryTokenType::kUnderscore);
    QueryToken times = QueryToken(QueryTokenType::kOperatorTimes);
    QueryToken equal = QueryToken(QueryTokenType::kEqual);
    QueryToken and_token = QueryToken(QueryTokenType::kKeywordAnd);
    QueryToken with = QueryToken(QueryTokenType::kKeywordWith);
    SECTION("next (basic queries)") {
        std::string_view query_string1{
                "procedure p, q; Select p.procName such that Calls (p, q)"};
        PQLValidator pql_validator1;
        auto tokens = pql_validator1.Validate(query_string1);
        QueryToken word_token_p = QueryToken(QueryTokenType::kWord, "p");
        QueryToken word_token_q = QueryToken(QueryTokenType::kWord, "q");
        std::vector<QueryToken> expected{
                decl_proc,    word_token_p, comma,     word_token_q, semicolon,
                select,       word_token_p, dot,       attr_proc,    such,
                that,         calls,        bracket_l, word_token_p, comma,
                word_token_q, bracket_r};
        REQUIRE(expected == tokens);
    }
    SECTION("next (queries of full suchthat-cl)") {
        std::string_view query_string2{
                "assign a; while w;Select a pattern a (\"x\", _) such that "
                "Parent* (w, a) and Next* (1, a)"};
        PQLValidator pql_validator2;
        auto tokens2 = pql_validator2.Validate(query_string2);
        QueryToken word_token_a = QueryToken(QueryTokenType::kWord, "a");
        QueryToken word_token_w = QueryToken(QueryTokenType::kWord, "w");
        QueryToken word_token_x = QueryToken(QueryTokenType::kWord, "x");
        QueryToken integer_token_1 = QueryToken(QueryTokenType::kInteger, "1");
        std::vector<QueryToken> expected{decl_assign,
                                         word_token_a,
                                         semicolon,
                                         decl_while,
                                         word_token_w,
                                         semicolon,
                                         select,
                                         word_token_a,
                                         pattern,
                                         word_token_a,
                                         bracket_l,
                                         quote,
                                         word_token_x,
                                         quote,
                                         comma,
                                         wildcard,
                                         bracket_r,
                                         such,
                                         that,
                                         parent,
                                         times,
                                         bracket_l,
                                         word_token_w,
                                         comma,
                                         word_token_a,
                                         bracket_r,
                                         and_token,
                                         next,
                                         times,
                                         bracket_l,
                                         integer_token_1,
                                         comma,
                                         word_token_a,
                                         bracket_r};
        REQUIRE(expected == tokens2);
    }
    SECTION("next (queries of with-cl)") {
        std::string_view query_string3{
                "procedure p, q;Select p such that Calls(p, q) with q.procName "
                "= \"Third\""};
        PQLValidator pql_validator3;
        auto tokens3 = pql_validator3.Validate(query_string3);
        QueryToken word_token_p = QueryToken(QueryTokenType::kWord, "p");
        QueryToken word_token_q = QueryToken(QueryTokenType::kWord, "q");
        QueryToken word_token_Third =
                QueryToken(QueryTokenType::kWord, "Third");
        std::vector<QueryToken> expected{
                decl_proc, word_token_p,     comma, word_token_q, semicolon,
                select,    word_token_p,     such,  that,         calls,
                bracket_l, word_token_p,     comma, word_token_q, bracket_r,
                with,      word_token_q,     dot,   attr_proc,    equal,
                quote,     word_token_Third, quote};
        REQUIRE(expected == tokens3);
    }
    SECTION("next (queries of full pattern-cl)") {
        std::string_view query_string4{
                "assign a; while w; if ifs; Select w pattern w(\"i\", _) "
                "pattern ifs(\"x\", _, _) and a ( _ , \"x\")"};
        PQLValidator pql_validator4;
        auto tokens4 = pql_validator4.Validate(query_string4);
        QueryToken word_token_a = QueryToken(QueryTokenType::kWord, "a");
        QueryToken word_token_w = QueryToken(QueryTokenType::kWord, "w");
        QueryToken word_token_ifs = QueryToken(QueryTokenType::kWord, "ifs");
        QueryToken word_token_i = QueryToken(QueryTokenType::kWord, "i");
        QueryToken word_token_x = QueryToken(QueryTokenType::kWord, "x");
        std::vector<QueryToken> expected{
                decl_assign,  word_token_a,   semicolon,    decl_while,
                word_token_w, semicolon,      decl_if,      word_token_ifs,
                semicolon,    select,         word_token_w, pattern,
                word_token_w, bracket_l,      quote,        word_token_i,
                quote,        comma,          wildcard,     bracket_r,
                pattern,      word_token_ifs, bracket_l,    quote,
                word_token_x, quote,          comma,        wildcard,
                comma,        wildcard,       bracket_r,    and_token,
                word_token_a, bracket_l,      wildcard,     comma,
                quote,        word_token_x,   quote,        bracket_r};
        REQUIRE(expected == tokens4);
    }
}
}  // namespace spa
