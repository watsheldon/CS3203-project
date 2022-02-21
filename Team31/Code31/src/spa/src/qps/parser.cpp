#include "parser.h"

#include <memory>

#include "generator.h"

namespace spa {
QueryObject Parser::parse(const std::string& value) {
    PQLValidator validator;
    auto tokens = validator.Validate(value);
    return generator_.Generate(tokens);
}
}  // namespace spa
