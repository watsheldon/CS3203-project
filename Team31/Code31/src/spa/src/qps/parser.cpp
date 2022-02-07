#include "parser.h"
#include "token.h"
#include "PQL_tokenizer.h"

using namespace spa;

QueryObject parser::parseQuery(const std::string &inputFile){
    PQLTokenizer tokenizer;
    std::vector<Token> tokenList = tokenizer.tokenize();
    if (validate(tokenList)){
        QueryObject query_object = {};
        return query_object;
    } else {
        throw
    }
}

bool parser::validate(std::vector<Token> tokenList){

}




