//
// Created by vanessa on 1/2/22.
//

#ifndef INC_21S2_CP_SPA_TEAM_31_EVALUATOR_H
#define INC_21S2_CP_SPA_TEAM_31_EVALUATOR_H


#include <list>
#include <string>
#include "Retriever.h"
#include "PQLEnums.h"

class Evaluator {

  public:

    void evaluateQuery(QueryObject queryObject);

    // evaluation list to be passed on to the retriever
    struct EvalList {
        std::vector<Query> noSynonyms;
        std::vector<std::vector<Query> > noTarget;
        std::vector<std::vector<std::string> > noTargetSyn;
        std::vector<std::vector<Query> > withTarget;
        std::vector<std::vector<std::string> > withTargetSyn;
    };

    // for processing purposes
    struct ProcList {
        std::vector<std::vector<Query> > grouped;
        std::vector<Query> withSynonyms;
    };

  private:
    std::pair<EvalList, ProcList> sortBySynonyms(QueryObject queryObject, EvalList evalList, ProcList procList);
    ProcList groupBySyn(ProcList procList);
    EvalList sortByTarget(EvalList evalList, ProcList procList, PQLEnums::TargetType target);
    EvalList getCommonSynonyms(EvalList evalList);
};


#endif //INC_21S2_CP_SPA_TEAM_31_EVALUATOR_H
