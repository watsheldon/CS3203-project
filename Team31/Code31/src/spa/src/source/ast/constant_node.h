#ifndef SRC_SPA_SRC_SOURCE_AST_CONSTANT_H_
#define SRC_SPA_SRC_SOURCE_AST_CONSTANT_H_

#include <string>
#include <cstring>

#include "indexed_node.h"

namespace spa {

class ConstantNode : public IndexedNode<ConstantNode> {
  public:
    explicit ConstantNode(std::string value);
    [[nodiscard]] std::string getValue() const;

  private:
    const std::string value_;
};

} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_AST_CONSTANT_H_
