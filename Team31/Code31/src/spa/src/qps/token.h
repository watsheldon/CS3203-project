#ifndef SRC_SPA_SRC_QPS_TOKEN_H_
#define SRC_SPA_SRC_QPS_TOKEN_H_
#include <string>

namespace spa{
enum class QueryTokenType{
    SEMICOLON, // ;
    COMMA,     // ,
    UNDERSCORE,  // _
    DOUBLEQUOTE, // "
    PLUS,  // +
    MINUS, // ,
    TIMES, // *
    DIVIDE, // /
    MODULO, // %
    LEFTBRACKET, // (
    RIGHTBRACKET, // )
    INTEGER,
    WORD,
};

const std::string_view Keyword(QueryTokenType token_type) {
    switch(token_type) {
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
    }
};
class QueryToken{
  private:
    QueryTokenType type;
    std::string string;
  public:
    QueryToken(const QueryTokenType &type) : type(type) {};
    QueryToken(const QueryTokenType &type, std::string string)
            : type(type), string(std::move(string)) {};
    std::string getString() {
        return this->string;
    }
    QueryTokenType getTokenType() {
        return this->type;
    }
};
}

#endif //INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_TOKEN_H_
