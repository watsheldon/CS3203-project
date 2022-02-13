#ifndef SPA_SRC_SPA_SRC_QPS_PATTERN_H_
#define SPA_SRC_SPA_SRC_QPS_PATTERN_H_

#include <string>

namespace spa {
class Pattern {
  private:
    std::string syn_assign;
    std::string ref1;
    std::string ref2;

  public:
    Pattern(std::string syn, std::string ref1, std::string ref2)
            : syn_assign(syn), ref1(ref1), ref2(ref2) {}

    std::string getSynonym() { return this->syn_assign; }

    std::string getRef1() { return this->ref1; }

    std::string getRef2() { return this->ref2; }
};
}

#endif  // SPA_SRC_SPA_SRC_QPS_PATTERN_H_