#ifndef SRC_SPA_SRC_QPS_PARSER_H_
#define SRC_SPA_SRC_QPS_PARSER_H_
#include "query_object.h"
#include "PQL_tokenizer.h"
#include "token.h"
#include <vector>

namespace spa {

    class parser{
        QueryObject parseQuery(const std::string& inputFile);
        std::vector<Declaration> parseDeclarations(std::vector<Token> tokenList, int endPos);
        Select parseSelect(std::vector<Token> tokenList, int startPos, std::vector<Declaration>);
        bool checkIdent(std::string s);
        DeclarationType getType(Token token);
    };
}

#endif //SPA_TEAM31_CODE31_SRC_SPA_SRC_QPS_PARSER_H_