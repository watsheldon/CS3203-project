#include "PQL_tokenizer.h"

#include <string>
#include <cctype>
#include <stdexcept>

namespace spa {
PQLTokenizer::PQLTokenizer(const std::string &inputFile) : currentPosition(0), queryString(inputFile) {}

int PQLTokenizer::getCurrChar() {
    if (currentPosition >= queryString.length()) {
        throw std::out_of_range("Out of string boundary");
    }
    return queryString[currentPosition];
}

int PQLTokenizer::getNextChar() {
    int nextPos = currentPosition + 1;
    if (nextPos >= queryString.length()) {
        throw std::out_of_range("Out of string boundary");
    }
    return queryString[nextPos];
}

Token PQLTokenizer::getNextToken() {
    int currChar = getCurrChar();
    while (isspace(currChar)) {
        currentPosition++;
        currChar = getCurrChar();
    }
    std::string str;
    str.push_back(currChar);
    if (str == ",") {
        currentPosition++;
        return Token(QueryTokenType::COMMA, ",");
    } else if (str == ";") {
        currentPosition++;
        return Token(QueryTokenType::SEMICOLON, ";");
    } else if (str == "_") {
        return Token(QueryTokenType::UNDERSCORE, "_");
    } else if (!isalnum(currChar)) {
        throw std::invalid_argument("Invalid character in query!");
    } else {
        std::string word;
        word.push_back(currChar);
        int nextChar = getNextChar();
        while (isalnum(nextChar)) {
            word.push_back(nextChar);
            currentPosition++;
        }
        return Token(QueryTokenType::WORD, word);
    }
}

std::vector<Token> PQLTokenizer::tokenize(const std::string &inputFile) {

    std::vector<Token> tokenList;
    while (currentPosition < queryString.length()) {
        Token nextToken = getNextToken();
        tokenList.push_back(nextToken);
    }
    return tokenList;
}
}