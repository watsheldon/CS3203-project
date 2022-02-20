#include "parser.h"

#include <cctype>
#include <stdexcept>

#include "PQL_tokenizer.h"
#include "query_token.h"
#include "select_cl.h"

namespace spa {
Parser::Parser(const std::string &inputFile) : queryPath(inputFile){};

QueryObject Parser::parse() { PQLValidator validator(queryPath); }
}  // namespace spa
