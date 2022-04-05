#include "source_processor.h"

#include <memory>
#include <utility>

#include "ast/abstract_syntax_tree.h"
#include "validator.h"

namespace spa {
SourceProcessor::SourceProcessor(const std::string &filename) noexcept
        : source_path_(filename) {}
std::unique_ptr<AbstractSyntaxTree> SourceProcessor::Parse() noexcept {
    Validator validator(source_path_);
    auto tokens = validator.Validate();
    if (tokens.empty()) return {};
    auto ast = std::make_unique<AbstractSyntaxTree>(std::move(tokens));
    if (ast->GetRoot() == nullptr) return nullptr;
    return std::move(ast);
}
}  // namespace spa
