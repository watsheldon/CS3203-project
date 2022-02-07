//
// Created by vanessa on 7/2/22.
//

#ifndef INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_EVALUATOR_DATA_RETRIEVER_DATA_RETRIEVER_H_
#define INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_EVALUATOR_DATA_RETRIEVER_DATA_RETRIEVER_H_

#include <string>
#include "../../evaluator/evaluator.h"
#include "../../../pkb/program_knowledge_base.h"

namespace spa {

class data_retriever {

  public:
    std::string GetData(const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr,
                                             spa::FilterType filter_type,
                                             spa::ParamsType params_type,
                                             std::vector<std::string> params);
};

}

#endif //INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_EVALUATOR_DATA_RETRIEVER_DATA_RETRIEVER_H_
