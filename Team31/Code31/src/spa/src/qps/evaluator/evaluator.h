//
// Created by vanessa on 1/2/22.
//

#ifndef INC_21S2_CP_SPA_TEAM_31_EVALUATOR_H
#define INC_21S2_CP_SPA_TEAM_31_EVALUATOR_H

#include <vector>
#include <string>
#include "to_scrap/retriever.h"
#include "pql_enums.h"

namespace spa {

class evaluator {

  public:

    void EvaluateQuery(const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr, QueryObject query_object);

  private:
    std::pair<evaluator::EvalList, std::vector<Query> > SortBySynonyms(QueryObject query_object);
    evaluator::EvalList GetConnectedQueries(std::vector<Query> &query_group);

    struct EvalList {
        bool has_target;
        std::unordered_map<spa::ParamsType, std::vector<std::string> > params_map;
        std::unordered_map<spa::FilterType, spa::ParamsType> filter_map;
        std::vector<std::string> all_synonyms;
    };

    std::vector<std::string> GetResult(const EvalList &no_synonyms,
                                       const std::vector<evaluator::EvalList> &groups,
                                       const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr);
};

}

#endif //INC_21S2_CP_SPA_TEAM_31_EVALUATOR_H
