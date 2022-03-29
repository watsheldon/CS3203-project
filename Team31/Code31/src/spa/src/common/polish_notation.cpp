#include "polish_notation.h"

#include <cassert>
#include <stack>
#include <utility>

namespace spa {
PolishNotation::PolishNotation(
        const std::vector<PolishNotationNode>& expr) noexcept {
    // Convert to reverse Polish Notation for ease of implementation
    std::vector<PolishNotationNode> pn;
    std::stack<PolishNotationNode, std::vector<PolishNotationNode>> s;
    for (auto node : expr) {
        if (node.type == ExprNodeType::kVariable ||
            node.type == ExprNodeType::kConstant) {
            pn.emplace_back(node);
        } else {
            HandleOperator(node, pn, s);
        }
    }
    while (!s.empty()) {
        pn.emplace_back(s.top());
        s.pop();
    }

    expr_ = std::move(pn);
}
bool PolishNotation::operator==(const PolishNotation& other) const noexcept {
    if (this == &other) return true;
    return expr_ == other.expr_;
}
bool PolishNotation::Contains(const PolishNotation& other) const noexcept {
    // Use KMP algorithm for pattern matching
    auto lps = ComputeLps(other);
    auto pattern = other.expr_;
    size_t i = 0;
    size_t j = 0;
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

std::vector<int> PolishNotation::GetAllVarIndices() const noexcept {
    std::vector<int> var_indices;
    for (auto node : expr_) {
        if (node.type == ExprNodeType::kVariable) {
            var_indices.emplace_back(std::get<int>(node.id));
        }
    }
    return var_indices;
}

std::vector<int> PolishNotation::ComputeLps(
        const PolishNotation& pattern) noexcept {
    // Compute the longest proper prefix suffix array
    // Helper method for KMP
    auto expr = pattern.expr_;
    std::vector<int> lps(expr.size());
    lps[0] = 0;
    int len = 0;
    for (int i = 1; i < lps.size();) {
        if (expr[i] == expr[len]) {
            lps[i++] = ++len;
            continue;
        }
        if (len == 0) {
            lps[i++] = 0;
            continue;
        }
        len = lps[len - 1];
    }
    return lps;
}
}  // namespace spa
