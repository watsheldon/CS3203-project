#ifndef SRC_SPA_SRC_POLISH_NOTATION_H_
#define SRC_SPA_SRC_POLISH_NOTATION_H_

#include <vector>

#include "polish_notation_node.h"

namespace spa {

class PolishNotation {
  public:
    explicit PolishNotation(std::vector<PolishNotationNode> &&expr);
    bool operator==(const PolishNotation &other) const;
    [[nodiscard]] bool SupersetOf(const PolishNotation &other) const;
    std::vector<int> GetAllVarIndices() const;

  private:
    std::vector<PolishNotationNode> expr_;
};

}  // namespace spa

#endif  // SRC_SPA_SRC_POLISH_NOTATION_H_
