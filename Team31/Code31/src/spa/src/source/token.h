#ifndef SRC_SPA_SRC_SOURCE_TOKEN_H_
#define SRC_SPA_SRC_SOURCE_TOKEN_H_

#include <array>
#include <string>
#include <string_view>
#include <utility>

#include "common/map.h"

namespace spa {
enum class SourceTokenType {
    kKeywordProcedure = 1,
    kKeywordRead,
    kKeywordPrint,
    kKeywordCall,
    kKeywordWhile,
    kKeywordIf,
    kKeywordThen,
    kKeywordElse,
    kBracketL,
    kBracketR,
    kBraceL,
    kBraceR,
    kAssignEqual,
    kOperatorPlus,
    kOperatorMinus,
    kOperatorTimes,
    kOperatorDivide,
    kOperatorModulo,
    kCondNot,
    kCondAnd,
    kCondOr,
    kRelLt,
    kRelLeq,
    kRelEq,
    kRelNeq,
    kRelGt,
    kRelGeq,
    kSemicolon,
    kName,
    kInteger,
};
using namespace std::literals::string_view_literals;
static constexpr const std::array<std::string_view, 30> kTypeStringMap{
        {"procedure"sv, "read"sv, "print"sv, "call"sv, "while"sv, "if"sv,
         "then"sv,      "else"sv, "("sv,     ")"sv,    "{"sv,     "}"sv,
         "="sv,         "+"sv,    "-"sv,     "*"sv,    "/"sv,     "%"sv,
         "!"sv,         "&&"sv,   "||"sv,    "<"sv,    "<="sv,    "=="sv,
         "!="sv,        ">"sv,    ">="sv,    ";"sv,    {},        {}}};
constexpr std::string_view GetSourceKeyword(
        SourceTokenType token_type) noexcept {
    return kTypeStringMap[static_cast<int>(token_type) - 1];
}
using NameTokenPair = std::pair<std::string_view, spa::SourceTokenType>;
static constexpr const auto kStringTypeMap = []() constexpr {
    std::array<NameTokenPair, kTypeStringMap.size() - 2> result{};
    for (int i = static_cast<int>(SourceTokenType::kKeywordProcedure);
         i <= static_cast<int>(SourceTokenType::kSemicolon); ++i) {
        result[i - 1].second = SourceTokenType{i};
        result[i - 1].first = GetSourceKeyword(result[i - 1].second);
    }
    return result;
}
();
constexpr const auto StringToSourceType =
        Map<std::string_view, SourceTokenType, kStringTypeMap.size()>{
                kStringTypeMap};

struct Token {
    const SourceTokenType type;
    const std::string value;

    explicit Token(const SourceTokenType &t) : type(t) {}
    Token(const SourceTokenType &t, std::string v)
            : type(t), value(std::move(v)) {}
    bool operator==(const Token &other) const {
        if (type != other.type) return false;
        return type < SourceTokenType::kName || value == other.value;
    }
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_TOKEN_H_
