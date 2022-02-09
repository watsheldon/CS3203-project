#include "polish_notation.h"

#include <utility>

namespace spa {
PolishNotation::PolishNotation(std::list<PolishNotationNode>&& expr)
        : expr_(std::move(expr)) {
    // todo: do the actual conversion
}
bool PolishNotation::operator==(const PolishNotation& other) const {
    if (this == &other) return true;
    return expr_ == other.expr_;
}
bool PolishNotation::SupersetOf(const PolishNotation& other) const {
    // essentially a substring problem
    return *this == other;  // placeholder for now
}
}  // namespace spa
