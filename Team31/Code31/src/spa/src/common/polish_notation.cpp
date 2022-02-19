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
bool PolishNotation::Contains(const PolishNotation& other) const {
    // Use KMP algorithm for pattern matching
    auto lps = ComputeLps(other);
    auto pattern = other.GetExpr();
    int i = 0;
    int j = 0;
    while (i < expr_.size()) {
        if (expr_[i] == pattern[j]) {
            i++;
            j++;
            if (j == pattern.size()) {
                return true;
            }
        } else {
            if (j == 0) {
                i++;
            } else {
                j = lps[j - 1];
            }
        }
    }
    return false;
}

std::vector<int> PolishNotation::GetAllVarIndices() const {
    std::vector<int> var_indices;
    for (auto node : expr_) {
        if (node.type == ExprNodeType::kVariable) {
            var_indices.emplace_back(node.id);
        }
    }
    return var_indices;
}

const std::vector<PolishNotationNode>& PolishNotation::GetExpr() const {
    return expr_;
}

std::vector<int> PolishNotation::ComputeLps(
        const PolishNotation& pattern) const {
    // Compute the longest proper prefix suffix array
    // Helper method for KMP
    auto expr = pattern.GetExpr();
    std::vector<int> lps(expr.size());
    lps[0] = 0;
    int len = 0;
    for (int i = 1; i < lps.size();) {
        if (expr[i] == expr[len]) {
            lps[i++] = ++len;
            continue;
        } else {
            if (len == 0) {
                lps[i++] = 0;
                continue;
            } else {
                len = lps[len - 1];
                continue;
            }
        }
    }
    return lps;
}
}  // namespace spa
