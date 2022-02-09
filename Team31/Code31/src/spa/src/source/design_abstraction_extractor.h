#ifndef SRC_SPA_SRC_SOURCE_DESIGN_ABSTRACTION_EXTRACTOR_H_
#define SRC_SPA_SRC_SOURCE_DESIGN_ABSTRACTION_EXTRACTOR_H_

#include <memory>

#include "ast/abstract_syntax_tree.h"
#include "pkb/knowledge_base.h"

namespace spa {
class DesignAbstractionExtractor {
    using AST = std::shared_ptr<AbstractSyntaxTree>;

  public:
    explicit DesignAbstractionExtractor(AST ast);
    std::shared_ptr<KnowledgeBase> Extract();

  private:
    AST ast_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_DESIGN_ABSTRACTION_EXTRACTOR_H_
