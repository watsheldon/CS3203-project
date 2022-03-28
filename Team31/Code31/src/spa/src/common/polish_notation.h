#ifndef SRC_SPA_SRC_COMMON_POLISH_NOTATION_H_
#define SRC_SPA_SRC_COMMON_POLISH_NOTATION_H_

#include <cassert>
#include <stack>
#include <vector>

#include "polish_notation_node.h"

namespace spa {

class PolishNotation {
  public:
    explicit PolishNotation(
            const std::vector<PolishNotationNode> &expr) noexcept;
    bool operator==(const PolishNotation &other) const noexcept;
    [[nodiscard]] bool Contains(const PolishNotation &other) const noexcept;
    [[nodiscard]] std::vector<int> GetAllVarIndices() const noexcept;

  private:
    std::vector<PolishNotationNode> expr_;
    [[nodiscard]] static std::vector<int> ComputeLps(
            const PolishNotation &pattern) noexcept;
    static void HandleOperator(const PolishNotationNode &node,
                               std::vector<PolishNotationNode> &pn,
                               std::stack<PolishNotationNode> &s) {
        assert(node.type == ExprNodeType::kOperator);
        auto op_type = std::get<OperatorType>(node.id);
        if (op_type == OperatorType::kBracketL) {
            s.emplace(node);
            return;
        }
        if (op_type == OperatorType::kBracketR) {
            while (std::get<OperatorType>(s.top().id) !=
                   OperatorType::kBracketL) {
                pn.emplace_back(s.top());
                s.pop();
            }
            s.pop();
            return;
        }
        while (!s.empty() && s.top().HasHigherPrecedence(node)) {
            pn.emplace_back(s.top());
            s.pop();
        }
        s.emplace(node);
    }
};
}  // namespace spa

#endif  // SRC_SPA_SRC_COMMON_POLISH_NOTATION_H_
