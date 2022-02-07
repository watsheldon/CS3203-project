#include "token.h"

namespace spa{
Token::Token(QueryTokenType type, std::string string) : type(type), string(string){}


QueryTokenType Token::getTokenType(){
    return type;
}

std::string Token::getString(){
    return string;
}
}








