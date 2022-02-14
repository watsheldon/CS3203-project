#include "polish_notation_node.h"

#include <cassert>

namespace spa {
PolishNotationNode::PolishNotationNode(OperatorType opr)
        : type(ExprNodeType::kOperator), id(static_cast<int>(opr)) {}
PolishNotationNode::PolishNotationNode(ExprNodeType node_type, int id)
        : type(node_type), id(id) {
    if (id == 0) {
        assert(node_type != ExprNodeType::kVariable);
        assert(node_type != ExprNodeType::kConstant);
    }
}
bool PolishNotationNode::operator==(const PolishNotationNode &other) const {
    return type == other.type && id == other.id;
}
}  // namespace spa