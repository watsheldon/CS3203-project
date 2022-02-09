#ifndef SRC_SPA_SRC_POLISH_NOTATION_H_
#define SRC_SPA_SRC_POLISH_NOTATION_H_

#include <list>

#include "polish_notation_node.h"

namespace spa {

class PolishNotation {
  public:
    explicit PolishNotation(std::list<PolishNotationNode> &&expr);
    bool operator==(const PolishNotation &other) const;
    [[nodiscard]] bool SupersetOf(const PolishNotation &other) const;

  private:
    // not storing the final output as string due to possibility that the
    // variable name may be very long, increasing compute and memory cost
    // can possibly convert to vector for the contiguous memory speedup
    std::list<PolishNotationNode> expr_;
};

}  // namespace spa

#endif  // SRC_SPA_SRC_POLISH_NOTATION_H_
