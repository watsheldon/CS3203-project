//
// Created by vanessa on 1/2/22.
//

#include "retriever.h"
#include "result_table.h"
#include <vector>
#include "pql_enums.h"
#include "evaluator.h"

retriever::retriever(PQLEnums::TargetType tg) {

    this->target = tg;

}

bool retriever::retrieve(evaluator::EvaluationList evalList) {

    // handle queries with no synonyms
    bool terminate = handleNoSynonyms(evalList.noSynonyms);

    // handle queries with no targets in their parameters
    if (!terminate) {
        terminate = handleNoTargets(evalList.noTarget, evalList.noTargetSyn, this->target);
    }
    return terminate;
}

bool retriever::handleNoSynonyms(std::vector<Query> queryList) {
    bool terminate;

    for (int i = 0; i < queryList.size(); i++) {
        bool result = getBool(queryList[i].getQueryType(), queryList[i].getParams());
        if (!result) {
            terminate = true;
        }
    }
    return terminate;
}

bool retriever::handleNoTargets(std::vector<Query> queryList, std::vector<std::vector<std::string> > headers) {

    bool terminate;
    std::vector<std::string> result;

    for (int i = 0; i < queryList.size(); i++) {
        auto rt = new result_table(headers[i]);
        for (int j = 0; j < queryList.size(); j++) {
            std::string queryType = queryList[i].getQueryType(); // Follows, Modifies, Pattern
            std::string params = queryList[i].getParams();
            result = callPKB(rt, queryType, params);
            rt->merge(result, this->target);
        }
        result = getResult(*rt);
        if (result.empty()) {
            terminate = true;
        }
    }
    return terminate;
}

std::vector<std::string> retriever::retrieve2(std::vector<Query> queryList, std::vector<std::vector<std::string> > headers) {

    std::vector<std::string> result;

    for (int i = 0; i < queryList.size(); i++) {
        auto rt = new result_table(headers[i]);
        for (int j = 0; j < queryList[i].size(); j++) {
            std::string queryType = queryList[i][j].getQueryType(); // Follows, Modifies, Pattern
            std::string params = queryList[i][j].getParams();

            result = callPKB(rt, queryType, params);
            rt->merge(result, target);
        }
        result = getResult(*rt);
    }

    return result;
}

std::vector<std::string> retriever::getResult(result_table rt) {

    std::vector<std::string> result;

    for (int i = 0; i < rt.getHeaders().size(); i ++) {
        if (this->target == rt.getHeaders()[i]) {
            result = rt.getTableContents()[i];
        }
    }

    return result;
}

// can change types to wrapper
bool retriever::getBool(std::string queryType, std::string params) {

    bool result;

    // call PKB methods
    // most likely the 'exists' methods

    return result;
}

std::vector<std::string> retriever::callPKB(result_table *rt, std::string queryType, std::string params) {

    std::vector<std::string> result;

    // call PKB methods

    return result;

}

// for minimal iteration only
std::vector<std::string> retriever::getSimpleQuery() {

    std::vector<std::string> result;

    switch (this->target) {
        case PQLEnums::ALL:
            result = PKB.getAll(STATEMENT);
        case PQLEnums::ASSIGN:
            result = PKB.getAll(ASSIGN);
        case PQLEnums::WHILE:
            result = PKB.getAll(WHILE);
        case PQLEnums::IF:
            result = PKB.getAll(IF);
        case PQLEnums::READ:
            result = PKB.getAll(READ);
        case PQLEnums::CONTAINER:
            result = PKB.getAll(CONTAINER);
        case PQLEnums::PRINT:
            result = PKB.getAll(PRINT);
        case PQLEnums::CALL:
            result = PKB.getAll(CALL);
        case PQLEnums::PROCEDURE:
            result = PKB.toName(PKB.getAll(PROCEDURE), PROCEDURE);
        case PQLEnums::VARIABLE:
            result = PKB.toName(PKB.getAll(VARIABLE), VARIABLE);
        case PQLEnums::CONSTANT:
            result = PKB.toName(PKB.getAll(CONSTANT), CONSTANT);
        default:
            throw std::runtime_error("Target type is invalid!");
    }
    return result;
}
