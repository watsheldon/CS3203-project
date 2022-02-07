//
// Created by vanessa on 7/2/22.
//

#ifndef INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_EVALUATOR_DATA_RETRIEVER_FOLLOWS_RETRIEVER_H_
#define INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_EVALUATOR_DATA_RETRIEVER_FOLLOWS_RETRIEVER_H_

#include <string>
#include "../../../pkb/program_knowledge_base.h"
#include "../pql_enums.h"

namespace spa {

    std::string GetData(const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr,
                        spa::ParamsType params_type,
                        std::vector<std::string> params);

};

}

#endif //INC_21S2_CP_SPA_TEAM_31_TEAM31_CODE31_SRC_SPA_SRC_QPS_EVALUATOR_DATA_RETRIEVER_FOLLOWS_RETRIEVER_H_
