//
// Created by vanessa on 7/2/22.
//

#include "follows_retriever.h"
#include <vector>

using namespace spa;

std::string follows_retriever::GetData(const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr,
                                       spa::ParamsType params_type,
                                       std::vector<std::string> params) {
    std::string result;

    switch (params_type) {
        case ParamsType::SINGLE_ARG:
            // call PKB API
        case ParamsType::DOUBLE_ARG:
            // call PKB API
        default:
            break;
    }

    return result;
}
