#ifndef SRC_SPA_SRC_QPS_TOKEN_H_
#define SRC_SPA_SRC_QPS_TOKEN_H_
#include <string>

namespace spa{
    enum class QueryTokenType{
      SEMICOLON, // ;
      COMMA,     // ,
      UNDERSCORE,  // _

      WORD,
    };

    class Token{
      private:
        QueryTokenType type;
        std::string string;
      public:
        Token(QueryTokenType type, std::string string);
        std::string getString();
        QueryTokenType getTokenType();
    };
}

#endif //INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_TOKEN_H_
