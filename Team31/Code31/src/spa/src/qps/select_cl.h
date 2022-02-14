#ifndef SRC_SPA_SRC_QPS_SELECT_H_
#define SRC_SPA_SRC_QPS_SELECT_H_
#include <string>
#include "declaration_cl.h"

namespace spa {
class SelectClause{
  private:
    DeclarationType type;
    std::string synonym;
  public:
    void setSelectType(DeclarationType t) {
        type = t;
    }
    void setSynonym(std::string string) {
        synonym = string;
    }
    DeclarationType getSelectType() {
        return this->type;
    }
    std::string getSelectSynonym() {
        return this->synonym;
    }
};
}
#endif //INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_SELECT_H_
