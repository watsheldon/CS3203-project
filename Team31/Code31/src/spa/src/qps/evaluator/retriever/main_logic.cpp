//
// Created by vanessa on 7/2/22.
//

#include "main_logic.h"
#include "follows_retriever.h"

using namespace spa;

// handle both arguments w synonyms differently!!
// implement early termination
/**
std::string main_logic::GetData(const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr,
                                     spa::FilterType filter_type,
                                     spa::ParamsType params_type,
                                     std::vector<std::string> params) {

    std::string result;

    switch (filter_type) {
        case FilterType::FOLLOWS:
            result = follows_retriever::GetData(pkb_ptr, params_type, params);
            break;
        case FilterType::FOLLOWS_T:
        case FilterType::PARENT:
        case FilterType::PARENT_T:
        case FilterType::USES:
        case FilterType::MODIFIES:
        case FilterType::PATTERN:
        default:
    }

    // consider the cases where we have to return GetSimpleQuery even if other clauses are present

}*/

// for minimal iteration; rename it; fix type
std::pair<std::vector<int>, std::vector<std::string> >
spa::main_logic::GetSimpleQuery(const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr,
                                     spa::DeclarationType target) {

    std::vector<int> result_int;
    std::vector<std::string> result_string;

    switch (target) {
        case spa::STMT:
            result_int = pkb_ptr->GetAllStmtEntities(kStmt);
            break;
        case spa::ASSIGN:
            result_int = pkb_ptr->GetAllStmtEntities(kAssignStmt);
            break;
        case spa::WHILE:
            result_int = pkb_ptr->GetAllStmtEntities(kWhileStmt);
            break;
        case spa::IF:
            result_int = pkb_ptr->GetAllStmtEntities(kIfStmt);
            break;
        case spa::READ:
            result_int = pkb_ptr->GetAllStmtEntities(kReadStmt);
            break;
        //case spa::CONTAINER:
        //    result_int = pkb_ptr->GetAllStmtEntities(kStmtLst);
        //    break;
        case spa::PRINT:
            result_int = pkb_ptr->GetAllStmtEntities(kPrintStmt);
            break;
        case spa::CALL:
            result_int = pkb_ptr->GetAllStmtEntities(kCallStmt);
            break;
        case spa::PROCEDURE:
            result_string = pkb_ptr->GetAllStringEntities(kProc);
            break;
        case spa::VARIABLE:
            result_string = pkb_ptr->GetAllStringEntities(kVar);
            break;
        case spa::CONSTANT:
            result_string = pkb_ptr->GetAllStringEntities(kConst);
            break;
        default:
            break;
    }
    return std::make_pair(result_int, result_string);
}

