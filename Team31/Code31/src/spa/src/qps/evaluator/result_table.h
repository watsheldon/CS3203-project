//
// Created by vanessa on 1/2/22.
//

#ifndef INC_21S2_CP_SPA_TEAM_31_RESULTTABLE_H
#define INC_21S2_CP_SPA_TEAM_31_RESULTTABLE_H

#include "pql_enums.h"
#include <string>
#include <map>
#include <vector>

namespace spa {

class result_table {

  public:

    explicit result_table(std::vector<std::string> headers);

    std::vector<std::vector<std::string> > GetTableContents();
    std::vector<std::string> GetHeaders();

    //void Merge(std::vector<std::string> result, spa::TargetType target);

  private:

    std::vector<std::string> table_header;
    std::vector<std::vector<std::string> > table_contents;

};

}

#endif //INC_21S2_CP_SPA_TEAM_31_RESULTTABLE_H