//
// Created by vanessa on 1/2/22.
//

#ifndef INC_21S2_CP_SPA_TEAM_31_EVALUATOR_H
#define INC_21S2_CP_SPA_TEAM_31_EVALUATOR_H

#include <vector>
#include <string>
#include "retriever.h"
#include "pql_enums.h"

class evaluator {

public:

    void evaluateQuery(QueryObject queryObject);

    // evaluation list to be passed on to the retriever
    struct EvaluationList {
        std::vector<Query> noSynonyms;
        std::vector<std::vector<Query> > noTarget;
        std::vector<std::vector<std::string> > noTargetSyn;
        std::vector<std::vector<Query> > withTarget;
        std::vector<std::vector<std::string> > withTargetSyn;
    };

    // for processing purposes
    struct ProcessingList {
        std::vector<std::vector<Query> > grouped;
        std::vector<Query> withSynonyms;
    };

private:
    std::pair<EvaluationList, ProcessingList> sortBySynonyms(QueryObject queryObject,
                                                             EvaluationList evalList, ProcessingList procList);
    ProcessingList groupBySyn(ProcessingList procList);
    EvaluationList sortByTarget(EvaluationList evalList, ProcessingList procList, PQLEnums::TargetType target);
    EvaluationList getCommonSynonyms(EvaluationList evalList);
};


#endif //INC_21S2_CP_SPA_TEAM_31_EVALUATOR_H
