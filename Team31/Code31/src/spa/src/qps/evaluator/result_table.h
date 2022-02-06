//
// Created by vanessa on 1/2/22.
//

#ifndef INC_21S2_CP_SPA_TEAM_31_RESULTTABLE_H
#define INC_21S2_CP_SPA_TEAM_31_RESULTTABLE_H

#include "pql_enums.h"
#include <string>
#include <map>
#include <vector>

class result_table {

public:


    explicit result_table(std::vector<std::string> headers);

    std::vector<std::vector<std::string> > getTableContents();
    std::vector<std::string> getHeaders();

    void merge(std::vector<std::string> result, PQLEnums::TargetType target);

private:

    std::vector<std::string> tableHeader;
    std::vector<std::vector<std::string> > tableContents;

};


#endif //INC_21S2_CP_SPA_TEAM_31_RESULTTABLE_H
