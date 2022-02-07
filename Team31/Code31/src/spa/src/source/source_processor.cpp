#include "source_processor.h"
#include "pkb/knowledge_base.h"
#include "validator.h"
#include "abstract_syntax_tree.h"
#include "design_abstraction_extractor.h"

namespace spa {
SourceProcessor::SourceProcessor(const std::string &filename)
        : source_path_(filename) {}
std::shared_ptr<KnowledgeBase> SourceProcessor::Parse() {
    Validator validator(source_path_);
    auto tokens = validator.Validate();
    auto ast = std::make_shared<AbstractSyntaxTree>(tokens);
    DesignAbstractionExtractor dae(ast);
    auto pkb = dae.Extract();
    return pkb;
}
}
