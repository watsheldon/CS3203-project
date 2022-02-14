#ifndef SRC_SPA_SRC_SOURCE_TOKEN_H_
#define SRC_SPA_SRC_SOURCE_TOKEN_H_

#include <string>
#include <utility>

namespace spa {
enum class SourceTokenType {
    kKeywordProcedure,
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

constexpr std::string_view Keyword(SourceTokenType token_type) {
    switch (token_type) {
        case SourceTokenType::kKeywordProcedure:
            return "procedure";
        case SourceTokenType::kKeywordRead:
            return "read";
        case SourceTokenType::kKeywordPrint:
            return "print";
        case SourceTokenType::kKeywordCall:
            return "call";
        case SourceTokenType::kKeywordWhile:
            return "while";
        case SourceTokenType::kKeywordIf:
            return "if";
        case SourceTokenType::kKeywordThen:
            return "then";
        case SourceTokenType::kKeywordElse:
            return "else";
        case SourceTokenType::kBracketL:
            return "(";
        case SourceTokenType::kBracketR:
            return ")";
        case SourceTokenType::kBraceL:
            return "{";
        case SourceTokenType::kBraceR:
            return "}";
        case SourceTokenType::kAssignEqual:
            return "=";
        case SourceTokenType::kOperatorPlus:
            return "+";
        case SourceTokenType::kOperatorMinus:
            return "-";
        case SourceTokenType::kOperatorTimes:
            return "*";
        case SourceTokenType::kOperatorDivide:
            return "/";
        case SourceTokenType::kOperatorModulo:
            return "%";
        case SourceTokenType::kCondNot:
            return "!";
        case SourceTokenType::kCondAnd:
            return "&&";
        case SourceTokenType::kCondOr:
            return "||";
        case SourceTokenType::kRelLt:
            return "<";
        case SourceTokenType::kRelLeq:
            return "<=";
        case SourceTokenType::kRelEq:
            return "==";
        case SourceTokenType::kRelNeq:
            return "!=";
        case SourceTokenType::kRelGt:
            return ">";
        case SourceTokenType::kRelGeq:
            return ">=";
        case SourceTokenType::kSemicolon:
            return ";";
        case SourceTokenType::kName:
        case SourceTokenType::kInteger:
            return "";
    }
}

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
