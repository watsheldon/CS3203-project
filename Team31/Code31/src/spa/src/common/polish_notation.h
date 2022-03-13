#ifndef SRC_SPA_SRC_COMMON_POLISH_NOTATION_H_
#define SRC_SPA_SRC_COMMON_POLISH_NOTATION_H_

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
};

}  // namespace spa

#endif  // SRC_SPA_SRC_COMMON_POLISH_NOTATION_H_
