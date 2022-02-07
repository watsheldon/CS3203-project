//
// Created by vanessa on 1/2/22.
//

#ifndef INC_21S2_CP_SPA_TEAM_31_EVALUATOR_H
#define INC_21S2_CP_SPA_TEAM_31_EVALUATOR_H

#include <vector>
#include <string>
#include <unordered_map>
#include "pql_enums.h"
#include "../../pkb/program_knowledge_base.h"

namespace spa {

class evaluator {

  public:

    void EvaluateQuery(const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr, QueryObject query_object);

  private:
    /**
    struct EvalList {
        bool has_target;
        // convert params and filter types to int?
        std::unordered_map<ParamsType, std::vector<std::string> > params_map;
        std::unordered_map<FilterType, spa::ParamsType> filter_map;
        std::vector<std::string> all_synonyms;
    };

    std::pair<evaluator::EvalList, std::vector<Query> > SortBySynonyms(QueryObject query_object);
    std::vector<Query> GetConnectedQueries(std::vector<Query> with_synonyms);
    evaluator::EvalList GetEvaluationGroups(std::vector<Query> &query_group);


    std::vector<std::string> GetResult(const EvalList &no_synonyms,
                                       const std::vector<evaluator::EvalList> &groups,
                                       const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr);
    */
};

}

#endif //INC_21S2_CP_SPA_TEAM_31_EVALUATOR_H
