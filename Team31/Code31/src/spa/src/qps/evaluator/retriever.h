//
// Created by vanessa on 1/2/22.
//

#ifndef INC_21S2_CP_SPA_TEAM_31_RETRIEVER_H
#define INC_21S2_CP_SPA_TEAM_31_RETRIEVER_H


#include <string>
#include "result_table.h"
#include <array>
#include "pql_enums.h"
#include "evaluator.h"

class retriever {

public:
    std::vector<std::string> getSimpleQuery(PQLEnums::TargetType target);
    std::vector<std::vector<std::string>> retrieve(evaluator::EvalList evalList, PQLEnums::TargetType target);

  private:

    std::string callPKB(result_table rt, std::string queryType, std::string params);

};


#endif //INC_21S2_CP_SPA_TEAM_31_RETRIEVER_H
