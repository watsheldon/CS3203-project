#ifndef INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_PQL_TOKENIZER_H_
#define INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_PQL_TOKENIZER_H_

#include <string>
#include <vector>
#include "token.h"


namespace spa{
    class PQLTokenizer{
        int currentPosition;
        std::string queryString;
      public:
        explicit PQLTokenizer(const std::string& inputFile);
        std::vector<Token> tokenize(const std::string& inputFile);
        Token getNextToken();
        int getCurrChar();
        int getNextChar();
    };
}
#endif //INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_PQL_TOKENIZER_H_
