//
// Created by vanessa on 1/2/22.
//

#ifndef INC_21S2_CP_SPA_TEAM_31_RESULTTABLE_H
#define INC_21S2_CP_SPA_TEAM_31_RESULTTABLE_H

#include "PQLEnums.h"
#include <string>
#include <map>

class ResultTable {

public:


    explicit ResultTable(std::vector<std::string> headers);

    std::vector<std::vector<std::string> > getTableContents();

    void merge(std::string result, PQLEnums::TargetType target);

private:

    std::vector<std::string> tableHeader;
    std::vector<std::vector<std::string> > tableContents;

};


#endif //INC_21S2_CP_SPA_TEAM_31_RESULTTABLE_H
