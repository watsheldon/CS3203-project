#ifndef SRC_SPA_SRC_COMMON_POLISH_NOTATION_NODE_H_
#define SRC_SPA_SRC_COMMON_POLISH_NOTATION_NODE_H_

#include <variant>

namespace spa {
enum class ExprNodeType { kVariable, kConstant, kOperator };
enum class OperatorType : int {
    kPlus,
    kMinus,
    kTimes,
    kDivide,
    kModulo,
    kBracketL,
    kBracketR
};
constexpr bool OprHigherEqual(OperatorType op1, OperatorType op2) noexcept {
    if (op1 >= OperatorType::kTimes && op1 <= OperatorType::kModulo) {
        return true;
    }
    if (op1 == OperatorType::kBracketL || op1 == OperatorType::kBracketR) {
        return (op2 == OperatorType::kBracketL ||
                op2 == OperatorType::kBracketR);
    }
    return (op2 <= OperatorType::kMinus || op2 >= OperatorType::kBracketL);
}
struct PolishNotationNode {
    using ID = std::variant<int, OperatorType>;
    const ExprNodeType type;
    const ID id;
    explicit PolishNotationNode(ExprNodeType node_type, int id) noexcept;
    explicit PolishNotationNode(OperatorType opr) noexcept;
    bool operator==(const PolishNotationNode &other) const noexcept;
};

}  // namespace spa

#endif  // SRC_SPA_SRC_COMMON_POLISH_NOTATION_NODE_H_
