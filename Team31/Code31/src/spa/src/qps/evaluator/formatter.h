//
// Created by vanessa on 1/2/22.
//

#ifndef INC_21S2_CP_SPA_TEAM_31_FORMATTER_H
#define INC_21S2_CP_SPA_TEAM_31_FORMATTER_H

#include "result_table.h"

namespace spa {

class formatter {

  public:

    void Project(std::vector<std::string> result); // for minimal iteration

    std::string FormatResult(std::vector<std::string> raw_result);
};

}

#endif //INC_21S2_CP_SPA_TEAM_31_FORMATTER_H
