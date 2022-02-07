#include "design_abstraction_extractor.h"

#include <utility>

#include "pkb/knowledge_base.h"
#include "pkb/program_knowledge_base.h"

namespace spa {
spa::DesignAbstractionExtractor::DesignAbstractionExtractor(AST ast)
        : ast_(std::move(ast)) {}
std::shared_ptr<KnowledgeBase> DesignAbstractionExtractor::Extract() {
    auto pkb = std::make_shared<ProgramKnowledgeBase>(ast_->getInitEntities());
    pkb->Compile();
    return pkb;
}
}
