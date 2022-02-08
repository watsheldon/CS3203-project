#ifndef SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_NODE_H_
#define SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_NODE_H_

#include <memory>
#include <vector>

namespace spa {
class AbstractSyntaxTreeNode {
  public:
    template<typename T>
    using SharedPtrVec = std::vector<std::shared_ptr<T>>;
    virtual ~AbstractSyntaxTreeNode() = 0;
};
} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_NODE_H_
