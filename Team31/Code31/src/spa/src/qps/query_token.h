#ifndef SRC_SPA_SRC_QPS_TOKEN_H_
#define SRC_SPA_SRC_QPS_TOKEN_H_
#include <string>

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
    kSemicolon,       // ;
    kComma,           // ,
    kUnderscore,      // _
    kQuote,           // "
    kOperatorPlus,    // +
    kOperatorMinus,   // ,
    kOperatorTimes,   // *
    kOperatorDivide,  // /
    kOperatorModulo,  // %
    kBracketL,        // (
    kBracketR,        // )
    kWord,
    kInteger,
};

inline std::string_view Keyword(QueryTokenType token_type) {
    switch (token_type) {
        case QueryTokenType::kDeclStmt:
            return "stmt";
        case QueryTokenType::kDeclRead:
            return "read";
        case QueryTokenType::kDeclPrint:
            return "print";
        case QueryTokenType::kDeclCall:
            return "call";
        case QueryTokenType::kDeclWhile:
            return "while";
        case QueryTokenType::kDeclIf:
            return "if";
        case QueryTokenType::kDeclAssign:
            return "assign";
        case QueryTokenType::kDeclVariable:
            return "variable";
        case QueryTokenType::kDeclConstant:
            return "constant";
        case QueryTokenType::kDeclProcedure:
            return "procedure";
        case QueryTokenType::kKeywordSelect:
            return "Select";
        case QueryTokenType::kKeywordPattern:
            return "pattern";
        case QueryTokenType::kKeywordFollows:
            return "Follows";
        case QueryTokenType::kKeywordParent:
            return "Parent";
        case QueryTokenType::kKeywordUses:
            return "Uses";
        case QueryTokenType::kKeywordModifies:
            return "Modifies";
        case QueryTokenType::kKeywordCalls:
            return "Calls";
        case QueryTokenType::kKeywordNext:
            return "Next";
        case QueryTokenType::kKeywordAffects:
            return "Affects";
        case QueryTokenType::kSemicolon:
            return ";";
        case QueryTokenType::kComma:
            return ",";
        case QueryTokenType::kUnderscore:
            return "_";
        case QueryTokenType::kQuote:
            return "\"";
        case QueryTokenType::kOperatorPlus:
            return "+";
        case QueryTokenType::kOperatorMinus:
            return "-";
        case QueryTokenType::kOperatorTimes:
            return "*";
        case QueryTokenType::kOperatorDivide:
            return "/";
        case QueryTokenType::kOperatorModulo:
            return "%";
        case QueryTokenType::kBracketL:
            return "(";
        case QueryTokenType::kBracketR:
            return ")";
        case QueryTokenType::kKeywordSuch:
            return "such";
        case QueryTokenType::kKeywordThat:
            return "that";
        case QueryTokenType::kKeywordWith:
            return "with";
        case QueryTokenType::kKeywordAnd:
            return "and";
        case QueryTokenType::kWord:
        case QueryTokenType::kInteger:
            return "";
    }
};
struct QueryToken {
    const QueryTokenType type;
    const std::string value;
    explicit QueryToken(const QueryTokenType &type) : type(type){};
    QueryToken(const QueryTokenType &type, std::string string)
            : type(type), value(std::move(string)){};
    QueryToken(const QueryToken &other) = default;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_TOKEN_H_
