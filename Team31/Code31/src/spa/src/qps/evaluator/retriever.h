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

    retriever(PQLEnums::TargetType tg);
    bool retrieve(evaluator::EvaluationList evalList);
    std::vector<std::string> getSimpleQuery();
    std::vector<std::string> getResult(result_table rt);
    std::vector<std::string> retrieve2(std::vector<Query> queryList, std::vector<std::vector<std::string> > headers);
  private:

    PQLEnums::TargetType target;
    bool handleNoSynonyms(std::vector<Query> queryList);
    std::vector<std::string> callPKB(result_table *rt, std::string queryType, std::string params);
    bool handleNoTargets(std::vector<Query> queryList, std::vector<std::vector<std::string> > headers);
    bool getBool(std::string queryType, std::string params);
};


#endif //INC_21S2_CP_SPA_TEAM_31_RETRIEVER_H
