#ifndef INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_SELECT_H_
#define INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_SELECT_H_
#include <string>

namespace spa {
enum class SelectType{
    STMT,
    READ,
    PRINT,
    CALL,
    WHILE,
    IF,
    ASSIGN,
    VARIABLE,
    CONSTANT,
    PROCEDURE,
};

class Select{
  private:
    SelectType type;
    std::string synonym;
  public:
    Select(SelectType type, std::string synonym);
    SelectType getSelectType();
    std::string getSelectSynonym();
};
}
#endif //INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_SELECT_H_
