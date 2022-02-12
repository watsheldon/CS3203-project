#ifndef SRC_SPA_SRC_SOURCE_DESIGN_ABSTRACTION_EXTRACTOR_H_
#define SRC_SPA_SRC_SOURCE_DESIGN_ABSTRACTION_EXTRACTOR_H_

#include <memory>

#include "ast/abstract_syntax_tree.h"
#include "pkb/knowledge_base.h"

namespace spa {
class DesignAbstractionExtractor {
    using AST = std::unique_ptr<AbstractSyntaxTree>;

  public:
    std::unique_ptr<KnowledgeBase> Extract(AST ast);
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_DESIGN_ABSTRACTION_EXTRACTOR_H_
