#ifndef SRC_SPA_SRC_QPS_TOKEN_H_
#define SRC_SPA_SRC_QPS_TOKEN_H_
#include <string>

namespace spa {
enum class QueryTokenType {
    // Design entities
    STMT,
    READ,
    PRINT,
    CALL,
    WHILE,
    IF,
    ASSIGN,
    VARIABLE,
    CONSTANT,
    PROCEDURE,
    SELECT,
    SUCH,
    THAT,
    PATTERN,
    FOLLOWS,
    FOLLOWS_T,
    PARENT,
    PARENT_T,
    USES_S,
    USES_P,
    MODIFIES_S,
    MODIFIES_P,
    SEMICOLON,     // ;
    COMMA,         // ,
    UNDERSCORE,    // _
    DOUBLEQUOTE,   // "
    PLUS,          // +
    MINUS,         // ,
    TIMES,         // *
    DIVIDE,        // /
    MODULO,        // %
    LEFTBRACKET,   // (
    RIGHTBRACKET,  // )

    WORD,
    INTEGER,
};

std::string_view Keyword(QueryTokenType token_type) {
    switch (token_type) {
        case QueryTokenType::STMT:
            return "stmt";
        case QueryTokenType::READ:
            return "read";
        case QueryTokenType::PRINT:
            return "print";
        case QueryTokenType::CALL:
            return "call";
        case QueryTokenType::WHILE:
            return "while";
        case QueryTokenType::IF:
            return "if";
        case QueryTokenType::ASSIGN:
            return "assign";
        case QueryTokenType::VARIABLE:
            return "variable";
        case QueryTokenType::CONSTANT:
            return "constant";
        case QueryTokenType::PROCEDURE:
            return "procedure";
        case QueryTokenType::SELECT:
            return "Select";
        case QueryTokenType::PATTERN:
            return "pattern";
        case QueryTokenType::FOLLOWS:
            return "Follows";
        case QueryTokenType::FOLLOWS_T:
            return "Follows*";
        case QueryTokenType::PARENT:
            return "Parent";
        case QueryTokenType::PARENT_T:
            return "Parent*";
        case QueryTokenType::USES_S:
            return "Uses";
        case QueryTokenType::MODIFIES_S:
            return "Modifies";
        case QueryTokenType::SEMICOLON:
            return ";";
        case QueryTokenType::COMMA:
            return ",";
        case QueryTokenType::UNDERSCORE:
            return "_";
        case QueryTokenType::DOUBLEQUOTE:
            return "\"";
        case QueryTokenType::PLUS:
            return "+";
        case QueryTokenType::MINUS:
            return "-";
        case QueryTokenType::TIMES:
            return "*";
        case QueryTokenType::DIVIDE:
            return "/";
        case QueryTokenType::MODULO:
            return "%";
        case QueryTokenType::LEFTBRACKET:
            return "(";
        case QueryTokenType::RIGHTBRACKET:
            return ")";
        case QueryTokenType::SUCH:
            return "such";
        case QueryTokenType::THAT:
            return "that";
        case QueryTokenType::WORD:
        case QueryTokenType::INTEGER:
            return "";
    }
};
struct QueryToken {
    const QueryTokenType type;
    const std::string value;
    explicit QueryToken(const QueryTokenType &type) : type(type){};
    QueryToken(const QueryTokenType &type, std::string string)
            : type(type), value(std::move(string)){};
};
}  // namespace spa

#endif  // INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_TOKEN_H_
