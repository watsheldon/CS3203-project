#ifndef SPA_TEAM31_CODE31_SRC_SPA_SRC_QPS_PARSER_H_
#define SPA_TEAM31_CODE31_SRC_SPA_SRC_QPS_PARSER_H_
#include "query_object.h"
#include "PQL_tokenizer.h"
#include "token.h"
#include <vector>

namespace spa {

    class parser{
        QueryObject parseQuery(const std::string &inputFile);
        bool validate(std::vector<Token> tokenList);
        expect comma
        expect semicolon
        expect declaration
        expect "select"
    };
}

#endif //SPA_TEAM31_CODE31_SRC_SPA_SRC_QPS_PARSER_H_
