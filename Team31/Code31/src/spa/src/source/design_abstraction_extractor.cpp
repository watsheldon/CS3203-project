#include "design_abstraction_extractor.h"

#include <memory>

#include "pkb/knowledge_base.h"
#include "pkb/program_knowledge_base.h"

namespace spa {
std::unique_ptr<KnowledgeBase> DesignAbstractionExtractor::Extract(AST ast) {
    auto pkb = std::make_unique<ProgramKnowledgeBase>(ast->getInitEntities());
    pkb->Compile();
    return pkb;
}
}  // namespace spa
