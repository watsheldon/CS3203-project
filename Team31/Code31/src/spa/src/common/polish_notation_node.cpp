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
bool PolishNotationNode::HasHigherEqualPrecedence(
        const PolishNotationNode &other) const noexcept {
    if (other.type != type || type != ExprNodeType::kOperator) {
        assert(false);
    }
    auto op1 = std::get<OperatorType>(id);
    auto op2 = std::get<OperatorType>(other.id);
    if (op1 >= OperatorType::kTimes && op1 <= OperatorType::kModulo) {
        return true;
    }
    if (op1 == OperatorType::kBracketL || op1 == OperatorType::kBracketR) {
        return (op2 == OperatorType::kBracketL ||
                op2 == OperatorType::kBracketR);
    }
    return (op2 <= OperatorType::kMinus || op2 >= OperatorType::kBracketL);
}
}  // namespace spa
