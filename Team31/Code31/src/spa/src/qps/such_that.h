#ifndef SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_SUCHTHAT_H_
#define SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_SUCHTHAT_H_

#include <string>

namespace spa {
enum class SuchThatType{
    FOLLOWS,
    FOLLOWS_T,
    PARENT,
    PARENT_T,
    USES_S,
    USES_P,
    MODIFIES_S,
    MODIFIES_P,
};
class SuchThat{
  private:
    SuchThatType type;
    std::string ref1;
    std::string ref2;
  public :
    SuchThat(SuchThatType type, std::string ref1, std::string ref2)
        : type(type), ref1(ref1), ref2(ref2) {}

    SuchThatType getType() {
        return this->type;
    }

    std::string getRef1() {
        return this->ref1;
    }

    std::string getRef2() {
        return this->ref2;
    }
};
}
#endif //SPA_TEAM31_CODE31_SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_SUCHTHAT_H_
