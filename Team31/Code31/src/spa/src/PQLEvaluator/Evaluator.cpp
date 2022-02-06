#include <iostream>

//
// Created by vanessa on 1/2/22.
//

#include <set>
#include "Evaluator.h"
#include "Retriever.h"
#include "ResultTable.h"
#include "Formatter.h"
#include "DependencyGraph.h"
#include <vector>
#include <map>
#include "PQLEnums.h"

void Evaluator::evaluateQuery(QueryObject queryObject) {

    Retriever r;
    Formatter f;

    // put TargetType attribute in QueryObject class
    PQLEnums::TargetType target = queryObject.getTarget(); // implement getTarget() in QueryObject class

    if (queryObject.hasPattern() || queryObject.hasSuchThat()) {  // simplify to .hasQueries() in QueryObject class

        EvalList evalList;
        ProcList procList;

        // extract queries without synonyms
        std::pair<EvalList, ProcList> sortedBySyns = sortBySynonyms(queryObject, evalList, procList);

        // find dependencies/links among queries (with synonyms)
        ProcList grouped = groupBySyn(sortedBySyns.second);

        // separate queries (with synonyms) according to the presence of the query target in their parameters
        EvalList sortedByTarget = sortByTarget(sortedBySyns.first, grouped, target);

        // get common synonyms among the different groups of dependent queries (with synonyms)
        EvalList executionList = getCommonSynonyms(sortedByTarget);

        // retrieve results from PKB
        std::vector<std::vector<std::string> > rawResult = r.retrieve(executionList, target);

        // format results for projection
        std::string result = f.formatResult(rawResult, target);

        // for minimal iteration only
    } else {
        std::vector<std::string> result = r.getSimpleQuery(target);
        f.project(result);
    }

}

std::pair<Evaluator::EvalList, Evaluator::ProcList> Evaluator::sortBySynonyms(QueryObject queryObject, EvalList evalList, ProcList procList) {

    // get all queries in a vector
    std::vector<Query> allQueries = queryObject.getAllQueries(); // implemented in QueryObject class

    // separate into queries with synonyms and without
    for (int i = 0; i < allQueries.size(); i++) {
        if (allQueries[i].hasSynonyms()) {             // implement in Query class
            procList.withSynonyms.push_back(allQueries[i]);
        } else {
            evalList.noSynonyms.push_back(allQueries[i]);
        }
    }

    return std::make_pair(evalList, procList);
}

Evaluator::ProcList Evaluator::groupBySyn(Evaluator::ProcList procList) {

    DependencyGraph *g = new DependencyGraph(procList.withSynonyms.size());
    std::vector<Query> queriesCopy = procList.withSynonyms;

    // optimize
    for (int i = 0; i < procList.withSynonyms.size(); i++) {
        queriesCopy.erase(queriesCopy.begin() + i);
        for (int j = 0; j < queriesCopy.size(); j++) {
            if (procList.withSynonyms[i].sharesSynonym(queriesCopy[j])) {     // implemented in Query class
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

Evaluator::EvalList Evaluator::sortByTarget(EvalList evalList, ProcList procList, PQLEnums::TargetType target) {

    for (int i = 0; i < procList.grouped.size(); i++) {
        if (procList.grouped[i].containsTarget(target)) {
            evalList.withTarget.push_back(procList.grouped[i]);
        } else {
            evalList.noTarget.push_back(procList.grouped[i]);
        }
    }
    return evalList;
}


// returns vector of shared synonyms corresponding to each query group
Evaluator::EvalList Evaluator::getCommonSynonyms(Evaluator::EvalList evalList) {

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


