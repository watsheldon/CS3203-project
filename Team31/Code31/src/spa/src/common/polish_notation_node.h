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
struct PolishNotationNode {
    using ID = std::variant<int, OperatorType>;
    const ExprNodeType type;
    const ID id;
    explicit PolishNotationNode(ExprNodeType node_type, ID id) noexcept;
    // higher or equal precedence
    bool HasHigherPrecedence(const PolishNotationNode &other) const noexcept;
    bool operator==(const PolishNotationNode &other) const noexcept;
};

}  // namespace spa

#endif  // SRC_SPA_SRC_COMMON_POLISH_NOTATION_NODE_H_
