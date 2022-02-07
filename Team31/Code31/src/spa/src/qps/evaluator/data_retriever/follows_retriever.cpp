//
// Created by vanessa on 7/2/22.
//

#include "follows_retriever.h"

using namespace spa;

std::string GetData(const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr, spa::ParamsType params_type,
                    std::vector<std::string> params) {
    switch (params_type) {
        case ParamsType::SINGLE_ARG:
            // call PKB API
        case ParamsType::DOUBLE_ARG:
            // call PKB API
        default:
            throw std::runtime_error("something!");
    }
}