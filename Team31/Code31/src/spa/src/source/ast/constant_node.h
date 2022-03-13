#ifndef SRC_SPA_SRC_SOURCE_AST_CONSTANT_NODE_H_
#define SRC_SPA_SRC_SOURCE_AST_CONSTANT_NODE_H_

#include "indexed_node.h"

namespace spa {

class ConstantNode : public IndexedNode<ConstantNode> {
  public:
    ConstantNode() noexcept = default;
};

}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_AST_CONSTANT_NODE_H_
