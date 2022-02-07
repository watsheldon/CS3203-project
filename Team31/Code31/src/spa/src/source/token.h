#ifndef SRC_SPA_SRC_SOURCE_TOKEN_H_
#define SRC_SPA_SRC_SOURCE_TOKEN_H_

#include <string>
#include <utility>

namespace spa {
enum SourceTokenType {
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
        case kKeywordProcedure:return "procedure";
        case kKeywordRead:return "read";
        case kKeywordPrint:return "print";
        case kKeywordCall:return "call";
        case kKeywordWhile:return "while";
        case kKeywordIf:return "if";
        case kKeywordThen:return "then";
        case kKeywordElse:return "else";
        case kBracketL:return "(";
        case kBracketR:return ")";
        case kBraceL:return "{";
        case kBraceR:return "}";
        case kAssignEqual:return "=";
        case kOperatorPlus:return "+";
        case kOperatorMinus:return "-";
        case kOperatorTimes:return "*";
        case kOperatorDivide:return "/";
        case kOperatorModulo:return "%";
        case kCondNot:return "!";
        case kCondAnd:return "&&";
        case kCondOr:return "||";
        case kRelLt:return "<";
        case kRelLeq:return "<=";
        case kRelEq:return "==";
        case kRelNeq:return "!=";
        case kRelGt:return ">";
        case kRelGeq:return ">=";
        case kSemicolon:return ";";
        case kName:
        case kInteger:return "";
    }
}

struct Token {
    const SourceTokenType type;
    const std::string value;

    explicit Token(const SourceTokenType &t) : type(t) {}
    Token(const SourceTokenType &t, std::string v)
            : type(t), value(std::move(v)) {}
    bool operator==(const Token &other) {
        if (type != other.type)
            return false;
        return type < kName || value == other.value;
    }
};
}

#endif //SRC_SPA_SRC_SOURCE_TOKEN_H_
