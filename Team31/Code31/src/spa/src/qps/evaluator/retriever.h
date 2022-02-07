//
// Created by vanessa on 1/2/22.
//

#ifndef INC_21S2_CP_SPA_TEAM_31_RETRIEVER_H
#define INC_21S2_CP_SPA_TEAM_31_RETRIEVER_H

#include <string>
#include "result_table.h"
#include <array>
#include "pql_enums.h"
#include "evaluator.h"

namespace spa {

// relocate retriever class!!

class retriever {

  public:

    retriever(spa::TargetType tg);
    std::vector<std::string> GetSimpleQuery(const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr);
    std::vector<std::string> getResult(result_table rt);

  private:
    spa::TargetType target;
    std::vector<std::string> callPKB(result_table *rt, std::string queryType, std::string params);
};

}

#endif //INC_21S2_CP_SPA_TEAM_31_RETRIEVER_H
