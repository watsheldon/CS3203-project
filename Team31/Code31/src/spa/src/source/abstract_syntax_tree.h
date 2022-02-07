#ifndef SRC_SPA_SRC_SOURCE_ABSTRACT_SYNTAX_TREE_H_
#define SRC_SPA_SRC_SOURCE_ABSTRACT_SYNTAX_TREE_H_

#include <memory>
#include <vector>

#include "token.h"
#include "pkb/knowledge_base.h"

namespace spa {
class AbstractSyntaxTree {
  public:
    using SharedVecToken = std::shared_ptr<std::vector<Token>>;
    explicit AbstractSyntaxTree(SharedVecToken tokens);
    std::shared_ptr<BasicEntities> getInitEntities() const;
  private:
    SharedVecToken tokens_;
};
}

#endif //SRC_SPA_SRC_SOURCE_ABSTRACT_SYNTAX_TREE_H_
