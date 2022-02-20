#include "parser.h"

#include <memory>
#include <string>
#include <vector>

#include "PQL_tokenizer.h"
#include "query_token.h"
#include "select_cl.h"

namespace spa {
Parser::Parser(const std::string &inputFile) : queryPath(inputFile){};

QueryObject Parser::parse() {
    PQLValidator validator(queryPath);
    return QueryObject(false, std::vector<std::unique_ptr<Synonym>>(), nullptr,
                       nullptr, nullptr);
}
}  // namespace spa
