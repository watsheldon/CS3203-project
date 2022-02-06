//
// Created by vanessa on 1/2/22.
//

#ifndef INC_21S2_CP_SPA_TEAM_31_RETRIEVER_H
#define INC_21S2_CP_SPA_TEAM_31_RETRIEVER_H


#include <string>
#include "ResultTable.h"
#include <array>
#include "PQLEnums.h"
#include "Evaluator.h"

class Retriever {

public:
    std::vector<std::string> getSimpleQuery(PQLEnums::TargetType target);
    std::vector<std::vector<std::string>> retrieve(Evaluator::EvalList evalList, PQLEnums::TargetType target);

  private:

    std::string callPKB(ResultTable rt, std::string queryType, std::string params);

};


#endif //INC_21S2_CP_SPA_TEAM_31_RETRIEVER_H
