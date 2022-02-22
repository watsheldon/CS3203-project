#ifndef SRC_SPA_SRC_QPS_PARSER_H_
#define SRC_SPA_SRC_QPS_PARSER_H_

#include <filesystem>
#include <fstream>

#include "PQL_tokenizer.h"
#include "PQL_validator.h"
#include "generator.h"
#include "query_object.h"
#include "query_token.h"

namespace spa {
class Parser {
  private:
    Generator generator_;

  public:
    QueryObject parse(const std::string& value);
    //        QueryObject parseQuery(const std::string& inputFile);
    //        Declaration parseDeclaration(std::vector<Token> tokenList);
    //        Select parseSelect(std::vector<Token> tokenList, int startPos,
    //        Declaration declaration); bool checkIdent(std::string s);
    //        DeclarationType getType(Token token);
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_PARSER_H_
