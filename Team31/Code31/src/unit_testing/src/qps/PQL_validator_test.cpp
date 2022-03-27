#include "qps/PQL_validator.h"

#include <string_view>
#include <vector>

#include "catch.hpp"

namespace spa {
TEST_CASE("common/PQLValidator") {
    SECTION("next (basic queries)") {
        std::string_view query_string1{
                "procedure p, q; Select p.procName such that Calls (p, q)"};
        PQLValidator pql_validator1;
        auto tokens = pql_validator1.Validate(query_string1);
        QueryToken decl_proc = QueryToken(QueryTokenType::kDeclProcedure);
        QueryToken word_token_p = QueryToken(QueryTokenType::kWord, "p");
        QueryToken comma = QueryToken(QueryTokenType::kComma);
        QueryToken word_token_q = QueryToken(QueryTokenType::kWord, "q");
        QueryToken semicolon = QueryToken(QueryTokenType::kSemicolon);
        QueryToken decl_variable = QueryToken(QueryTokenType::kDeclVariable);
        QueryToken select = QueryToken(QueryTokenType::kKeywordSelect);
        QueryToken dot = QueryToken(QueryTokenType::kDot);
        QueryToken attr_proc = QueryToken(QueryTokenType::kAttrProc);
        QueryToken such = QueryToken(QueryTokenType::kKeywordSuch);
        QueryToken that = QueryToken(QueryTokenType::kKeywordThat);
        QueryToken calls = QueryToken(QueryTokenType::kKeywordCalls);
        QueryToken bracket_l = QueryToken(QueryTokenType::kBracketL);
        QueryToken bracket_r = QueryToken(QueryTokenType::kBracketR);
        std::vector<QueryToken> expected{
                decl_proc,    word_token_p, comma,     word_token_q, semicolon,
                select,       word_token_p, dot,       attr_proc,    such,
                that,         calls,        bracket_l, word_token_p, comma,
                word_token_q, bracket_r};
        REQUIRE(expected == tokens);
    }
}
}  // namespace spa
