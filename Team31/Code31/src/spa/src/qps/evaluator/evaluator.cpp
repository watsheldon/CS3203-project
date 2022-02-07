#include <iostream>

//
// Created by vanessa on 1/2/22.
//

#include <set>
#include "evaluator.h"
#include "to_scrap/retriever.h"
#include "result_table.h"
#include "formatter.h"
#include "dependency_graph.h"
#include <vector>
#include <map>
#include "pql_enums.h"
#include "../../pkb/program_knowledge_base.h"
#include "data_retriever/data_retriever.h"

using namespace spa;

void evaluator::EvaluateQuery(const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr, QueryObject query_object) {

    spa::TargetType target = query_object.getTarget(); // implement getTarget() in QueryObject class

    formatter f;
    auto r = new retriever(target);
    data_retriever dr;

    auto sorted = SortBySynonyms(query_object);
    auto no_synonyms = sorted.first;
    auto with_synonyms = sorted.second;

    if (!no_synonyms.empty() && !with_synonyms.empty()) {
        std::vector<EvalList> groups = GetConnectedQueries(with_synonyms);
        std::vector<std::string> result = GetResult(no_synonyms, groups, pkb_ptr);
        f.Project(result);

    } else {
        // for minimal iteration only; fit this in w the rest of the query results
        std::vector<std::string> result = dr.GetSimpleQuery(pkb_ptr, target);
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

evaluator::EvalList evaluator::GetConnectedQueries(std::vector<Query> &query_group) {

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

    data_retriever dr;

    auto rt = new result_table(no_synonyms.all_synonyms);

    for (const auto& filter : no_synonyms.filter_map) {
        auto params = filter.params_map[DOUBLE_ARG];
        auto result = dr.GetData(pkb_ptr, filter, DOUBLE_ARG, params);
        rt->Merge(result);
    }
}



