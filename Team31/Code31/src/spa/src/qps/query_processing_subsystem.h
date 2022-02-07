#ifndef SRC_SPA_SRC_QPS_QUERY_PROCESSING_SUBSYSTEM_H_
#define SRC_SPA_SRC_QPS_QUERY_PROCESSING_SUBSYSTEM_H_

#include "pkb/program_knowledge_base.h"

class QueryProcessingSubsystem {
  public:
    void use(const std::shared_ptr<spa::ProgramKnowledgeBase> &pkb_ptr);
    void evaluate(std::string_view query_string, std::list<std::string> &list);
  private:
    std::shared_ptr<spa::ProgramKnowledgeBase> pkb_;
};

#endif //SRC_SPA_SRC_QPS_QUERY_PROCESSING_SUBSYSTEM_H_
