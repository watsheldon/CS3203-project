#ifndef SPA_SRC_SPA_SRC_QPS_PATTERN_CL_H_
#define SPA_SRC_SPA_SRC_QPS_PATTERN_CL_H_

#include <string>

namespace spa {
class PatternClause {
  private:
    std::string syn_assign;
    std::string ref1;
    std::string ref2;

  public:
    void setSynonym(std::string synonym) {
        syn_assign = synonym;
    }
    void setRef1(std::string ref) {
        ref1 = ref;
    }
    void setRef2(std::string ref) {
        ref2 = ref;
    }
    std::string getSynonym() { return this->syn_assign; }

    std::string getRef1() { return this->ref1; }

    std::string getRef2() { return this->ref2; }
};
}

#endif  // SPA_SRC_SPA_SRC_QPS_PATTERN_CL_H_