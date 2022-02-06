//
// Created by vanessa on 1/2/22.
//

#include "retriever.h"
#include "result_table.h"
#include <vector>
#include "pql_enums.h"
#include "evaluator.h"

std::vector<std::vector<std::string> > retriever::retrieve(evaluator::EvalList evalList, PQLEnums::TargetType target) {

    bool terminate;

    // handle queries with no synonyms first as they'll evaluate to true/false
    std::vector<std::vector<std::string> > result;
    for (int i = 0; i < evalList.noSynonyms.size(); i++) {
        bool result = getBool(evalList.noSynonyms[i].getQueryType(), evalList.noSynonyms[i].getParams());
        if (!result) {
            terminate = true;
        }
    }

    // handle queries with no targets next as they'll also evaluate to true/false
    // but queries with synonyms will probably take more time than those without
    if (!terminate) {
        for (int i = 0; i < evalList.noTarget.size(); i++) {
            result_table rt = (const result_table &) new result_table(evalList.withTargetSyn[i]);
            for (int j = 0; j < evalList.noTarget.size(); j++) {
                std::string queryType = evalList.noTarget[i].getQueryType(); // Follows, Modifies, Pattern
                std::string params = evalList.noTarget[i].getParams();
                std::string result = callPKB(rt, queryType, params);
                rt.merge(result, target);
            }
            result = rt.getTableContents();
            if (result.empty()) {
                terminate = true;
            }
        }
    }

    if (evalList.withTarget.size() > 0) {

        // handle queries with synonyms and with targets
        if (!terminate) {
            for (int i = 0; i < evalList.withTarget.size(); i++) {
                result_table rt = (const result_table &) new result_table(evalList.withTargetSyn[i]);
                for (int j = 0; j < evalList.withTarget[i].size(); j++) {

                    std::string queryType = evalList.withTarget[i][j].getQueryType(); // Follows, Modifies, Pattern
                    std::string params = evalList.withTarget[i][j].getParams();

                    std::string result = callPKB(rt, queryType, params);
                    rt.merge(result, target);
                }
                result = rt.getTableContents();
            }

        }
        return result;
    } else {
        std::vector<std::string> result = r.getSimpleQuery(target);
        f.project(result);
    }



}

bool retriever::getBool(std::string queryType, std::string argType) {

    bool result;

    // call PKB methods

    return result;
}

std::string retriever::callPKB(result_table rt, std::string queryType, std::string params) {

    Result result;

    // call PKB methods

    return result;

}

// for minimal iteration only
std::vector<std::string> retriever::getSimpleQuery(PQLEnums::TargetType targetType) {

    std::vector<std::string> result;

    switch (targetType) {
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
