#include <iostream>

//
// Created by vanessa on 1/2/22.
//

#include <set>
#include "evaluator.h"
#include "retriever.h"
#include "result_table.h"
#include "formatter.h"
#include "dependency_graph.h"
#include <vector>
#include <map>
#include "pql_enums.h"

void evaluator::evaluateQuery(QueryObject queryObject) {

    // put TargetType attribute in QueryObject class
    PQLEnums::TargetType target = queryObject.getTarget(); // implement getTarget() in QueryObject class

    formatter f;
    bool terminate;
    auto r = new retriever(target);
    std::vector<std::string> rawResult;

    if (queryObject.hasPattern() || queryObject.hasSuchThat()) {  // simplify to .hasQueries() in QueryObject class

        EvaluationList evalList;
        ProcessingList procList;

        // extract queries without synonyms
        std::pair<EvaluationList, ProcessingList> sortedBySyns = sortBySynonyms(queryObject, evalList, procList);

        // group queries according to common synonyms
        ProcessingList grouped = groupBySyn(sortedBySyns.second);

        // sort query groups according to their possession of the query target in their parameters
        EvaluationList sortedByTarget = sortByTarget(sortedBySyns.first, grouped, target);

        // get common synonyms corresponding to the different query groups
        EvaluationList finalQueryList = getCommonSynonyms(sortedByTarget);

        // get results (true/false) for queries without synonyms and queries with no targets
        // if this returns false, terminate early
        terminate = r->retrieve(finalQueryList);

        if (!terminate && evalList.withTarget.size() > 0) {

            rawResult = r->retrieve2(finalQueryList.withTarget, finalQueryList.withTargetSyn);
            std::string result = f.formatResult(rawResult);

        } else if (!terminate && evalList.noTarget.size() == 0) {

            std::vector<std::string> result = r->getSimpleQuery();
            f.project(result);
        }

    // for minimal iteration only
    } else {
        std::vector<std::string> result = r->getSimpleQuery();
        f.project(result);
    }

}

std::pair<evaluator::EvaluationList,
evaluator::ProcessingList> evaluator::sortBySynonyms(QueryObject queryObject,
                                                     EvaluationList evalList, ProcessingList procList) {

    // get all queries in a vector
    std::vector<Query> allQueries = queryObject.getAllQueries(); // implemented in QueryObject class

    // separate into queries with synonyms and without
    for (int i = 0; i < allQueries.size(); i++) {
        if (allQueries[i].hasSynonyms()) {             // implement hasSynonyms() in Query class
            procList.withSynonyms.push_back(allQueries[i]);
        } else {
            evalList.noSynonyms.push_back(allQueries[i]);
        }
    }

    return std::make_pair(evalList, procList);
}

evaluator::ProcessingList evaluator::groupBySyn(evaluator::ProcessingList procList) {

    dependency_graph *g = new dependency_graph(procList.withSynonyms.size());
    std::vector<Query> withSynonymsCopy = procList.withSynonyms;

    // optimize
    for (int i = 0; i < procList.withSynonyms.size(); i++) {
        withSynonymsCopy.erase(withSynonymsCopy.begin() + i);
        for (int j = 0; j < withSynonymsCopy.size(); j++) {
            if (procList.withSynonyms[i].sharesSynonym(withSynonymsCopy[j])) {     // implement sharesSynonym in Query class
                g->addConnection(i, j + i + 1);
            }
        }
    }

    std::vector<std::vector<int> > connectedGroupsIndex = g->getConnectedNodes();

    // map back from Index to Query
    for (int i = 0; i < connectedGroupsIndex.size(); i++) {
        for (int j = 0; j < connectedGroupsIndex[i].size(); j++) {
            int index = connectedGroupsIndex[i][j];
            procList.grouped[i][j] = procList.withSynonyms[index];
        }
    }

    procList.withSynonyms.clear();

    return procList;
}

evaluator::EvaluationList evaluator::sortByTarget(EvaluationList evalList, ProcessingList procList, PQLEnums::TargetType target) {

    for (int i = 0; i < procList.grouped.size(); i++) {
        if (procList.grouped[i].containsTarget(target)) {       // implement containsTarget in Query
            evalList.withTarget.push_back(procList.grouped[i]);
        } else {
            evalList.noTarget.push_back(procList.grouped[i]);
        }
    }
    return evalList;
}


// returns vector of shared synonyms corresponding to each query group
evaluator::EvaluationList evaluator::getCommonSynonyms(evaluator::EvaluationList evalList) {

    for (int i = 0; i < evalList.noTarget.size(); i++) {
        std::vector<std::string> synonyms;
        for (int j = 0; j < evalList.noTarget[i].size(); j++) {
            std::string params = evalList.noTarget[i][j].getParams();
            std::string synonym = params.getSynonym();
            if (std::find(synonyms.begin(), synonyms.end(), synonym) == synonyms.end()) {
                synonyms.push_back(synonym);
            }
        }
        evalList.noTargetSyn.push_back(synonyms);
        synonyms.clear();
    }

    for (int i = 0; i < evalList.withTarget.size(); i++) {
        std::vector<std::string> synonyms;
        for (int j = 0; j < evalList.withTarget[i].size(); j++) {
            std::string params = evalList.withTarget[i][j].getParams();
            std::string synonym = params.getSynonym();
            if (std::find(synonyms.begin(), synonyms.end(), synonym) == synonyms.end()) {
                synonyms.push_back(synonym);
            }
        }
        evalList.withTargetSyn.push_back(synonyms);
        synonyms.clear();
    }

    return evalList;
}


