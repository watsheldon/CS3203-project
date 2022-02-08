#include "parser.h"
#include "token.h"
#include "PQL_tokenizer.h"
#include "select.h"
#include "declaration.h"
#include <stdexcept>
#include <cctype>

namespace spa{
QueryObject parser::parseQuery(const std::string& inputFile){
    std::string str = inputFile;
    PQLTokenizer tokenizer(str);
    std::vector<Token> tokenList = tokenizer.tokenize(str);

    int pos = 0;
    for(int i = 0; i < tokenList.size(); i++){
        auto token = tokenList[i];
        if (token.getString() == "Select") {
            pos = i;
            break;
        }
    }
    if (pos == 0) {
        throw std::invalid_argument("There must be a Select clause after Declaration!");
    }
    Declaration declaration = parseDeclaration(tokenList);
    Select select = parseSelect(tokenList, pos, declaration);
    QueryObject query_object(declaration, select);
    return query_object;
}


Declaration parser::parseDeclaration(std::vector<Token> tokenList){
    //std::vector<Declaration> declarations;
    Token token = tokenList[0];
    DeclarationType currType = parser::getType(token);
    std::string synonym = tokenList[1].getString();
    if(!checkIdent(synonym)) {
        throw std::invalid_argument("Synonym must be an alphanumeric string starting with a letter!");
    }
    if(tokenList[2].getString() != ";") {
        throw std::invalid_argument("Declaration clause must end with a semicolon!");
    }
    Declaration declaration(currType, synonym);
    //declarations.push_back(Declaration(currType, synonym));
    return declaration;
}

DeclarationType parser::getType(Token token){
    std::string str = token.getString();
    if (str == "stmt") {
        return DeclarationType::STMT;
    }
    if (str == "read") {
        return DeclarationType::READ;
    }
    if (str == "print") {
        return DeclarationType::PRINT;
    }
    if (str == "call") {
        return DeclarationType::CALL;
    }
    if (str == "while") {
        return DeclarationType::WHILE;
    }
    if (str == "if") {
        return DeclarationType::IF;
    }
    if (str == "assign") {
        return DeclarationType::ASSIGN;
    }
    if (str == "variable") {
        return DeclarationType::VARIABLE;
    }
    if (str == "constant") {
        return DeclarationType::CONSTANT;
    }
    if (str == "procedure") {
        return DeclarationType::PROCEDURE;
    }
    throw std::invalid_argument("Wrong design entity in declaration!");
}

Select parser::parseSelect(std::vector<Token> tokenList, int startPos, Declaration declaration){
    if (startPos >= tokenList.size() - 1) {
        throw std::out_of_range("There must be a synonym in Select clause!");
    }
    int synPos = startPos + 1;
    Token synToken = tokenList[synPos];
    //Declaration firstDeclaration = declarations[0];
    if (synToken.getString() != declaration.getDeclarationSynonym()) {
        throw std::invalid_argument("Select synonym must be consistent with declarations!");
    }
    Select select(declaration.getDeclarationType(), declaration.getDeclarationSynonym());
    return select;
}

bool parser::checkIdent(std::string s){
    int len = s.length();
    if (len == 0) {
        return false;
    }
    if(!isalpha(s[0])){
        return false;
    }
    for (int i = 1; i < len; i++){
        if(!isalnum(s[i])){
            return false;
        }
    }
    return true;
}
}




