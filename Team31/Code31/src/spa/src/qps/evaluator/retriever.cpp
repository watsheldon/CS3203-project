//
// Created by vanessa on 1/2/22.
//

#include "retriever.h"
#include "result_table.h"
#include <vector>
#include "pql_enums.h"
#include "evaluator.h"
#include "../../pkb/program_knowledge_base.h"

using namespace spa;

// relocate retriever class!!

retriever::retriever(spa::TargetType tg) {

    this->target = tg;

}

// move
std::vector<std::string> GetResult(evaluator::EvalList no_synonyms, std::vector<evaluator::EvalList> groups,
                                   const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr) {

    for (const auto& filter : no_synonyms.filter_map) {
        auto params = filter.params_map[DOUBLE_ARG];
        auto result = GetPKBData(filter, DOUBLE_ARG, params);
    }

}

// remove
bool retriever::handleNoTargets(std::vector<Query> queryList, std::vector<std::vector<std::string> > headers) {

    bool terminate;
    std::vector<std::string> result;

    for (int i = 0; i < queryList.size(); i++) {
        auto rt = new result_table(headers[i]);
        for (int j = 0; j < queryList.size(); j++) {
            std::string queryType = queryList[i].getQueryType(); // Follows, Modifies, Pattern
            std::string params = queryList[i].getParams();
            result = callPKB(rt, queryType, params);
            rt->Merge(result, this->target);
        }
        result = getResult(*rt);
        if (result.empty()) {
            terminate = true;
        }
    }
    return terminate;
}

// remove
std::vector<std::string> retriever::retrieve2(std::vector<Query> queryList, std::vector<std::vector<std::string> > headers) {

    std::vector<std::string> result;

    for (int i = 0; i < queryList.size(); i++) {
        auto rt = new result_table(headers[i]);
        for (int j = 0; j < queryList[i].size(); j++) {
            std::string queryType = queryList[i][j].getQueryType(); // Follows, Modifies, Pattern
            std::string params = queryList[i][j].getParams();

            result = callPKB(rt, queryType, params);
            rt->Merge(result, target);
        }
        result = getResult(*rt);
    }

    return result;
}

// remove
std::vector<std::string> retriever::getResult(result_table rt) {

    std::vector<std::string> result;

    for (int i = 0; i < rt.GetHeaders().size(); i ++) {
        //if (this->target == rt.GetHeaders()[i]) {
        //    result = rt.GetTableContents()[i];
        //}
    }

    return result;
}

// remove
bool retriever::getBool(std::string queryType, std::string params) {

    bool result;

    // call PKB methods
    // most likely the 'exists' methods

    return result;
}

// remove
std::vector<std::string> retriever::callPKB(result_table *rt, std::string queryType, std::string params) {

    std::vector<std::string> result;

    // call PKB methods

    return result;

}

// for minimal iteration only
std::vector<std::string> retriever::GetSimpleQuery(const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr) {

    std::vector<std::string> result;

    switch (this->target) {
        case spa::ALL:
            result = pkb_ptr->getAll(ALL);
            break;
        case spa::ASSIGN:
            result = pkb_ptr->getAll(ASSIGN);
            break;
        case spa::WHILE:
            result = pkb_ptr->getAll(WHILE);
            break;
        case spa::IF:
            result = pkb_ptr->getAll(IF);
            break;
        case spa::READ:
            result = pkb_ptr->getAll(READ);
            break;
        case spa::CONTAINER:
            result = pkb_ptr->getAll(CONTAINER);
            break;
        case spa::PRINT:
            result = pkb_ptr->getAll(PRINT);
            break;
        case spa::CALL:
            result = pkb_ptr->getAll(CALL);
            break;
        case spa::PROCEDURE:
            result = pkb_ptr->toName(pkb_ptr->getAll(PROCEDURE), PROCEDURE);
            break;
        case spa::VARIABLE:
            result = pkb_ptr->toName(pkb_ptr->getAll(VARIABLE), VARIABLE);
            break;
        case spa::CONSTANT:
            result = pkb_ptr->toName(pkb_ptr->getAll(CONSTANT), CONSTANT);
            break;
        default:
            throw std::runtime_error("Target type is invalid!");
    }
    return result;
}
