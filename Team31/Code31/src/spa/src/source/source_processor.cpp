#include "source_processor.h"

#include <memory>

#include "ast/abstract_syntax_tree.h"
#include "design_abstraction_extractor.h"
#include "pkb/knowledge_base.h"
#include "validator.h"

namespace spa {
SourceProcessor::SourceProcessor(const std::string &filename)
        : source_path_(filename) {}
std::shared_ptr<KnowledgeBase> SourceProcessor::Parse() {
    Validator validator(source_path_);
    auto tokens = validator.Validate();
    auto ast = std::make_shared<AbstractSyntaxTree>(tokens);
    DesignAbstractionExtractor dae(ast);
    return dae.Extract();
}
}
