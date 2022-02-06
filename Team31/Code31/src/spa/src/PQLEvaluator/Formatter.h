//
// Created by vanessa on 1/2/22.
//

#ifndef INC_21S2_CP_SPA_TEAM_31_FORMATTER_H
#define INC_21S2_CP_SPA_TEAM_31_FORMATTER_H

#include "ResultTable.h"

class Formatter {

public:

    void project(std::vector<std::string> result); // for minimal iteration

    std::string formatResult(std::vector<std::vector<std::string>> rt, PQLEnums::TargetType targetType);
};


#endif //INC_21S2_CP_SPA_TEAM_31_FORMATTER_H
