//
// Created by vanessa on 7/2/22.
//

#include "data_retriever.h"
#include "follows_retriever.h"
#include "../../../pkb/program_knowledge_base.h"


// handle both arguments w synonyms differently!!
// implement early termination
std::string spa::data_retriever::GetData(const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr,
                                         spa::FilterType filter_type,
                                         spa::ParamsType params_type,
                                         std::vector<std::string> params) {

    std::string result;

    switch (filter_type) {
        case FilterType::FOLLOWS:
            result = spa::GetData(pkb_ptr, params_type, params);
            break;
        case FilterType::FOLLOWS_T:
        case FilterType::PARENT:
        case FilterType::PARENT_T:
        case FilterType::USES:
        case FilterType::MODIFIES:
        case FilterType::PATTERN:
        default:
            throw std::runtime_error("Something!");
    }

    // consider the cases where we have to return GetSimpleQuery even if other clauses are present

}

// for minimal iteration; rename it
std::vector<std::string> spa::data_retriever::GetSimpleQuery(const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr,
                                                             spa::TargetType target) {

    std::vector<std::string> result;

    switch (target) {
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

