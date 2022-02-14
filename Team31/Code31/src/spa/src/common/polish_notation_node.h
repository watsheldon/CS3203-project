#ifndef SRC_SPA_SRC_COMMON_POLISH_NOTATION_NODE_H_
#define SRC_SPA_SRC_COMMON_POLISH_NOTATION_NODE_H_

namespace spa {
enum class ExprNodeType {
    kVariable,
    kConstant,
    kBracketL,
    kBracketR,
    kOperator
};
enum class OperatorType : int { kPlus, kMinus, kTimes, kDivide, kModulo };
struct PolishNotationNode {
    const ExprNodeType type;
    const int id;
    explicit PolishNotationNode(OperatorType opr);
    explicit PolishNotationNode(ExprNodeType node_type, int id = 0);
    bool operator==(const PolishNotationNode &other) const;
};

}  // namespace spa

#endif  // SRC_SPA_SRC_COMMON_POLISH_NOTATION_NODE_H_
