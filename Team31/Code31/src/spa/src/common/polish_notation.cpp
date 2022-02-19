#include "polish_notation.h"

#include <stack>
#include <utility>

namespace spa {
PolishNotation::PolishNotation(std::vector<PolishNotationNode>&& expr) {
    // Convert to reverse Polish Notation for ease of implementation
    std::vector<PolishNotationNode> pn;
    std::stack<PolishNotationNode> s;
    for (auto node : expr) {
        switch (node.type) {
            case ExprNodeType::kVariable:
            case ExprNodeType::kConstant:
                pn.emplace_back(node);
                break;
            case ExprNodeType::kBracketL:
                s.push(node);
                break;
            case ExprNodeType::kBracketR:
                while (s.top().type != ExprNodeType::kBracketL) {
                    pn.emplace_back(s.top());
                    s.pop();
                }
                s.pop();
                break;
            case ExprNodeType::kOperator:
                while (!s.empty() && s.top().id >= node.id) {
                    pn.emplace_back(s.top());
                    s.pop();
                }
                s.push(node);
        }
    }
    while (!s.empty()) {
        pn.emplace_back(s.top());
        s.pop();
    }

    expr_ = std::move(pn);
}
bool PolishNotation::operator==(const PolishNotation& other) const {
    if (this == &other) return true;
    return expr_ == other.expr_;
}
bool PolishNotation::SupersetOf(const PolishNotation& other) const {
    // essentially a substring problem
    return *this == other;  // placeholder for now
}

std::vector<int> PolishNotation::GetAllVarIndices() const { return {}; }
}  // namespace spa
