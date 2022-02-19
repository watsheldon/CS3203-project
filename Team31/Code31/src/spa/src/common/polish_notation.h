#ifndef SRC_SPA_SRC_POLISH_NOTATION_H_
#define SRC_SPA_SRC_POLISH_NOTATION_H_

#include <vector>

#include "polish_notation_node.h"

namespace spa {

class PolishNotation {
  public:
    explicit PolishNotation(const std::vector<PolishNotationNode> &expr);
    bool operator==(const PolishNotation &other) const;
    [[nodiscard]] bool Contains(const PolishNotation &other) const;
    [[nodiscard]] std::vector<int> GetAllVarIndices() const;
    [[nodiscard]] const std::vector<PolishNotationNode> &GetExpr() const;

  private:
    std::vector<PolishNotationNode> expr_;
    std::vector<int> ComputeLps(const PolishNotation &pattern) const;
};

}  // namespace spa

#endif  // SRC_SPA_SRC_POLISH_NOTATION_H_
