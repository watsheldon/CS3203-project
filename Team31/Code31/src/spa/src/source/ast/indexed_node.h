#ifndef SRC_SPA_SRC_SOURCE_AST_INDEXED_NODE_H_
#define SRC_SPA_SRC_SOURCE_AST_INDEXED_NODE_H_

#include "abstract_syntax_tree_node.h"

namespace spa {
template<typename T>
class IndexedNode : public AbstractSyntaxTreeNode {
  public:
    IndexedNode();
    [[nodiscard]] int get_index() const;

  private:
    inline static int count_ = 0;
    const int index;
};
} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_AST_INDEXED_NODE_H_
