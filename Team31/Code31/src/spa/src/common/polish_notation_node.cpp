#include "polish_notation_node.h"

#include <cassert>

namespace spa {
PolishNotationNode::PolishNotationNode(ExprNodeType node_type, ID id) noexcept
        : type(node_type), id(id) {
    auto *ptr = std::get_if<int>(&id);
    assert(ptr == nullptr || *ptr != 0);
}
bool PolishNotationNode::operator==(
        const PolishNotationNode &other) const noexcept {
    return type == other.type && id == other.id;
}
bool PolishNotationNode::HasHigherPrecedence(
        const PolishNotationNode &other) const noexcept {
    if (type != ExprNodeType::kOperator ||
        other.type != ExprNodeType::kOperator) {
        assert(false);
    }
    auto op1 = std::get<OperatorType>(id);
    auto op2 = std::get<OperatorType>(other.id);
    if (op1 >= OperatorType::kTimes) {
        return true;
    }
    if (op1 <= OperatorType::kBracketR) {
        if (op2 <= OperatorType::kBracketR) {
            return true;
        }
        return false;
    }
    if (op2 >= OperatorType::kTimes) {
        return false;
    }
    return true;
}
}  // namespace spa
