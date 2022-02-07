//
// Created by vanessa on 7/2/22.
//

#include "data_retriever.h"
#include "follows_retriever.h"
#include "../../../pkb/program_knowledge_base.h"


// handle both arguments synonyms differently!!
std::string spa::data_retriever::GetData(const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr,
                                         spa::FilterType filter_type,
                                         spa::ParamsType params_type,
                                         std::vector<std::string> params) {
    switch (filter_type) {
        case FilterType::FOLLOWS:
            return spa::GetData(pkb_ptr, params_type, params);
        case FilterType::FOLLOWS_T:
        case FilterType::PARENT:
        case FilterType::PARENT_T:
        case FilterType::USES:
        case FilterType::MODIFIES:
        case FilterType::PATTERN:
    }
}

