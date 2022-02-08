#ifndef SRC_SPA_SRC_SOURCE_AST_VARIABLE_NODE_H_
#define SRC_SPA_SRC_SOURCE_AST_VARIABLE_NODE_H_

#include <cstring>
#include <string>

#include "indexed_node.h"

namespace spa {
class VariableNode : public IndexedNode<VariableNode> {
  public:
    explicit VariableNode(std::string name);
    [[nodiscard]] std::string getName() const;

  private:
    const std::string name_;
};
} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_AST_VARIABLE_NODE_H_
