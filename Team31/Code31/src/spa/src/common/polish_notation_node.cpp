#include "polish_notation_node.h"

#include <cassert>

namespace spa {
PolishNotationNode::PolishNotationNode(OperatorType opr) noexcept
        : type(ExprNodeType::kOperator), id(static_cast<int>(opr)) {}
PolishNotationNode::PolishNotationNode(ExprNodeType node_type, int id) noexcept
        : type(node_type), id(id) {
    if (id == 0) {
        assert(node_type != ExprNodeType::kVariable);
        assert(node_type != ExprNodeType::kConstant);
    }
}
bool PolishNotationNode::operator==(
        const PolishNotationNode &other) const noexcept {
    return type == other.type && id == other.id;
}
}  // namespace spa
