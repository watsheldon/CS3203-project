#ifndef SRC_SPA_SRC_QPS_TOKEN_H_
#define SRC_SPA_SRC_QPS_TOKEN_H_

#include <array>
#include <string>
#include <string_view>
#include <utility>

namespace {
using namespace std::literals::string_view_literals;
}
namespace spa {
enum class QueryTokenType {
    // Design entities
    kDeclStmt,
    kDeclRead,
    kDeclPrint,
    kDeclCall,
    kDeclWhile,
    kDeclIf,
    kDeclAssign,
    kDeclVariable,
    kDeclConstant,
    kDeclProcedure,
    kKeywordSelect,
    kKeywordSuch,
    kKeywordThat,
    kKeywordPattern,
    kKeywordWith,
    kKeywordAnd,
    kKeywordFollows,
    kKeywordParent,
    kKeywordUses,
    kKeywordModifies,
    kKeywordCalls,
    kKeywordNext,
    kKeywordAffects,
    kAttrProc,
    kAttrVar,
    kAttrValue,
    kSemicolon,       // ;
    kComma,           // ,
    kUnderscore,      // _
    kQuote,           // "
    kOperatorPlus,    // +
    kOperatorMinus,   // -
    kOperatorTimes,   // *
    kOperatorDivide,  // /
    kOperatorModulo,  // %
    kBracketL,        // (
    kBracketR,        // )
    kAngleBracketL,   // <
    kAngleBracketR,   // >
    kDot,             // .
    kHashtag,         // #
    kWord,
    kInteger,
};
static constexpr const std::array<std::string_view, 43> kQueryTypeStringMap{
        {"stmt"sv,
         "read"sv,
         "print"sv,
         "call"sv,
         "while"sv,
         "if"sv,
         "assign"sv,
         "variable"sv,
         "constant"sv,
         "procedure"sv,
         "Select"sv,
         "such"sv,
         "that"sv,
         "pattern"sv,
         "with"sv,
         "and"sv,
         "Follows"sv,
         "Parent"sv,
         "Uses"sv,
         "Modifies"sv,
         "Calls"sv,
         "Next"sv,
         "Affects"sv,
         "procName"sv,
         "varName"sv,
         "value"sv,
         ";"sv,
         ","sv,
         "_"sv,
         R"(")"sv,
         "+"sv,
         "-"sv,
         "*"sv,
         "/"sv,
         "%"sv,
         "("sv,
         ")"sv,
         "<"sv,
         ">"sv,
         "."sv,
         "#"sv,
         {},
         {}}};
constexpr std::string_view GetQueryKeyword(QueryTokenType token_type) noexcept {
    return kQueryTypeStringMap[static_cast<int>(token_type)];
}
struct QueryToken {
    const QueryTokenType type;
    const std::string value;
    explicit QueryToken(const QueryTokenType &type) noexcept : type(type){};
    QueryToken(const QueryTokenType &type, std::string string) noexcept
            : type(type), value(std::move(string)){};
    QueryToken(const QueryToken &other) noexcept = default;
    bool operator==(const QueryToken &other) const noexcept {
        if (type != other.type) return false;
        return type < QueryTokenType::kWord || value == other.value;
    }
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_TOKEN_H_
