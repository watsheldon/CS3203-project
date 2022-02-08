#ifndef SRC_SPA_SRC_QPS_SELECT_H_
#define SRC_SPA_SRC_QPS_SELECT_H_
#include <string>
#include "declaration.h"

namespace spa {
class Select{
  private:
    DeclarationType type;
    std::string synonym;
  public:
    Select(DeclarationType d_type, std::string s_synonym);
    DeclarationType getSelectType();
    std::string getSelectSynonym();
};
}
#endif //INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_SELECT_H_