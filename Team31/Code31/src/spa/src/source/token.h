#ifndef SRC_SPA_SRC_SOURCE_TOKEN_H_
#define SRC_SPA_SRC_SOURCE_TOKEN_H_

#include <string>

namespace spa {
enum TokenType {
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
    kEqual,
    kOperatorPlus,
    kOperatorMinus,
    kOperatorTimes,
    kOperatorDivide,
    kOperatorModulo,
    kCondNot,
    kCondAnd,
    kCondOr,
    kRelGt,
    kRelGeq,
    kRelLt,
    kRelLeq,
    kRelEq,
    kRelNeq,
    kName,
    kInteger,
    kSemicolon,
};

struct Token {
    const TokenType type;
    const std::string value;
    bool operator==(const Token &other) {
        if (type == other.type)
            return type < kName || value == other.value;
        return false;
    }
};
}

#endif //SRC_SPA_SRC_SOURCE_TOKEN_H_
