#include <iostream>

//
// Created by vanessa on 1/2/22.
//

#include <set>
#include "evaluator.h"
#include "result_table.h"
#include "formatter.h"
#include "dependency_graph.h"
#include <vector>
#include <map>
#include "pql_enums.h"
#include "../../pkb/program_knowledge_base.h"
#include "retriever/main_logic.h"

using namespace spa;

void evaluator::EvaluateQuery(const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr, QueryObject query_object) {

    spa::TargetType target = query_object.getTarget();

    formatter f;
    main_logic re;

    // sort queries by their possession of synonyms
    auto sorted = SortBySynonyms(query_object);
    auto no_synonyms = sorted.first;
    auto with_synonyms = sorted.second;

    if (!no_synonyms.empty() && !with_synonyms.empty()) {
        // use undirected graphs to get queries connected by the same synonyms
        std::vector<std::vector<Query> > connected_groups = GetConnectedQueries(with_synonyms);

        // transform into evaluation groups // can use a class to represent the different groups + attributes
        std::vector<EvalList> eval_groups = GetEvaluationGroups(connected_groups);
        std::vector<std::string> result = GetResult(no_synonyms, eval_groups, pkb_ptr);
        //f.Project(result);

    } else {
        // MINIMAL ITERATION
        std::pair<std::vector<int>, std::vector<std::string> > result = re.GetSimpleQuery(pkb_ptr, target);
        f.Project(result);
    }
}

std::pair<evaluator::EvalList, std::vector<Query> > evaluator::SortBySynonyms(QueryObject query_object) {

    evaluator::EvalList no_synonyms;
    std::vector<Query> with_synonyms;

    for (const auto& query : query_object.getPattern()) {
        if (!query.hasSynonym()) {
            spa::FilterType filter_type = PATTERN;
            std::string params = getParams(query);

            no_synonyms.params_map[DOUBLE_ARG].push_back(params);
            no_synonyms.filter_map[filter_type].push_back(DOUBLE_ARG);
        } else {
            with_synonyms.push_back(query);
        }
    }

    for (const auto& query : query_object.getSuchThat()) {
        if (!query.hasSynonym()) {
            spa::FilterType filter_type = getFilterType(query);
            std::string params = getParams(query);

            no_synonyms.params_map[DOUBLE_ARG].push_back(params);
            no_synonyms.filter_map[filter_type].push_back(DOUBLE_ARG);
        } else {
            with_synonyms.push_back(query);
        }
    }

    return std::make_pair(no_synonyms, with_synonyms);
}

std::vector<Query> evaluator::GetConnectedQueries(std::vector<Query> with_synonyms) {

    dependency_graph *g = new dependency_graph(with_synonyms.size());
    std::vector<Query> withSynonymsCopy = with_synonyms;

    // optimize
    for (int i = 0; i < with_synonyms.size(); i++) {
        withSynonymsCopy.erase(withSynonymsCopy.begin() + i);
        for (int j = 0; j < withSynonymsCopy.size(); j++) {
            if (with_synonyms[i].sharesSynonym(withSynonymsCopy[j])) {     // implement sharesSynonym in Query class
                g->addConnection(i, j + i + 1);
            }
        }
    }

    std::vector<std::vector<int> > connected_groups_index = g->GetConnectedNodes();
    std::vector<std::vector<Query> > connected_groups;

    // map back from Index to Query
    for (int i = 0; i < connected_groups_index.size(); i++) {
        for (int j = 0; j < connected_groups_index[i].size(); j++) {
            int index = connected_groups_index[i][j];
            connected_groups.grouped[i][j] = with_synonyms[index];
        }
    }

    return connected_groups;
}

evaluator::EvalList evaluator::GetEvaluationGroups(std::vector<Query> &query_group) {

    evaluator::EvalList connected_queries;

    for (const auto& query : query_group) {
        spa::FilterType filter_type = getFilterType(query);
        std::string params = getParams(query);
        std::string synonyms = getSynonyms(params);
        spa::ParamsType params_type = getParamsType(params);

        if (params.containsTarget()) {
            connected_queries.has_target = true;
        }

        connected_queries.all_synonyms.push_back(synonyms);

        connected_queries.params_map[params_type].push_back(params);
        connected_queries.filter_map[filter_type].push_back(params_type);
    }
    return connected_queries;
}

std::vector<std::string> evaluator::GetResult(const evaluator::EvalList& no_synonyms,
                                              const std::vector<evaluator::EvalList>& groups,
                                   const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr) {

    main_logic re;

    auto rt = new result_table(no_synonyms.all_synonyms);

    for (const auto& filter : no_synonyms.filter_map) {
        auto params = filter.params_map[DOUBLE_ARG];
        auto result = re.GetData(pkb_ptr, filter, DOUBLE_ARG, params);
        rt->Merge(result);
    }
}



