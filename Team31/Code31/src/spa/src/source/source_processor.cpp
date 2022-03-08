#include "source_processor.h"

#include <memory>

#include "ast/abstract_syntax_tree.h"
#include "validator.h"

namespace spa {
SourceProcessor::SourceProcessor(const std::string &filename)
        : source_path_(filename) {}
std::unique_ptr<AbstractSyntaxTree> SourceProcessor::Parse() {
    Validator validator(source_path_);
    auto tokens = validator.Validate();
    if (tokens.empty()) return {};
    return std::make_unique<AbstractSyntaxTree>(std::move(tokens));
}
}  // namespace spa
