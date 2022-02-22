#ifndef SRC_SPA_SRC_QPS_PARSER_H_
#define SRC_SPA_SRC_QPS_PARSER_H_

#include <filesystem>
#include <fstream>
#include <string_view>

#include "PQL_tokenizer.h"
#include "PQL_validator.h"
#include "generator.h"
#include "query_object.h"
#include "query_token.h"

namespace spa {
class Parser {
  public:
    QueryObject Parse(std::string_view query);

  private:
    PQLValidator validator_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_PARSER_H_
