#include "parser.h"

#include <string_view>

#include "generator.h"

namespace spa {
QueryObject Parser::Parse(std::string_view query) {
    auto tokens = validator_.Validate(query);
    return Generator::Generate(tokens);
}
}  // namespace spa
