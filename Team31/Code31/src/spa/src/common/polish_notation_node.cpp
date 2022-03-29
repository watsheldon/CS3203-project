#include "polish_notation_node.h"

#include <cassert>

namespace spa {
PolishNotationNode::PolishNotationNode(ExprNodeType node_type, int id) noexcept
        : type(node_type), id(id) {
    assert(node_type != ExprNodeType::kOperator && id != 0);
}
PolishNotationNode::PolishNotationNode(OperatorType opr) noexcept
        : type(ExprNodeType::kOperator), id(opr) {}
bool PolishNotationNode::operator==(
        const PolishNotationNode &other) const noexcept {
    return type == other.type && id == other.id;
}
}  // namespace spa
