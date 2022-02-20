#ifndef SRC_SPA_SRC_QPS_PARSER_H_
#define SRC_SPA_SRC_QPS_PARSER_H_

#include <filesystem>
#include <fstream>

#include "PQL_tokenizer.h"
#include "PQL_validator.h"
#include "query_object.h"
#include "query_token.h"

namespace spa {
class Parser {
  private:
    std::filesystem::path queryPath;

  public:
    explicit Parser(const std::string& inputFile);
    QueryObject parse();
    //        QueryObject parseQuery(const std::string& inputFile);
    //        Declaration parseDeclaration(std::vector<Token> tokenList);
    //        Select parseSelect(std::vector<Token> tokenList, int startPos,
    //        Declaration declaration); bool checkIdent(std::string s);
    //        DeclarationType getType(Token token);
};
}  // namespace spa

#endif  // SPA_TEAM31_CODE31_SRC_SPA_SRC_QPS_PARSER_H_
