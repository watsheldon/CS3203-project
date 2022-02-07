#include "token.h"
using namespace spa;

Token::Token(TokenType type, std::string string) : type(type), string(string){}


TokenType Token::getTokenType(){
    return type;
}

std::string Token::getString(){
    return string;
}






