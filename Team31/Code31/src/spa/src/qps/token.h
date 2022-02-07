#ifndef SRC_SPA_SRC_QPS_TOKEN_H_
#define SRC_SPA_SRC_QPS_TOKEN_H_
#include <string>

namespace spa{
    enum class TokenType{
      SEMICOLON, // ;
      COMMA,     // ,
      UNDERSCORE,  // _

      WORD,
    };

    class Token{
      private:
        TokenType type;
        std::string string;
      public:
        Token(TokenType type, std::string string);
        std::string getString();
        TokenType getTokenType();
    };
}

#endif //INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_TOKEN_H_
