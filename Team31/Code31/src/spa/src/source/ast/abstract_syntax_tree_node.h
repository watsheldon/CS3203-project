#ifndef SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_NODE_H_
#define SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_NODE_H_

namespace spa {
class AbstractSyntaxTreeNode {
  public:
    AbstractSyntaxTreeNode() noexcept = default;
    virtual ~AbstractSyntaxTreeNode() = 0;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_AST_ABSTRACT_SYNTAX_TREE_NODE_H_
