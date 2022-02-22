#include "parser.h"

#include <memory>

#include "generator.h"

namespace spa {
QueryObject Parser::Parse(const std::string& query) {
    auto tokens = validator_.Validate(query);
    return Generator::Generate(tokens);
}
}  // namespace spa
