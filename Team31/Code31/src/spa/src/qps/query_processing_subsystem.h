#ifndef SRC_SPA_SRC_QPS_QUERY_PROCESSING_SUBSYSTEM_H_
#define SRC_SPA_SRC_QPS_QUERY_PROCESSING_SUBSYSTEM_H_

#include <list>
#include <string_view>

#include "parser.h"
#include "pkb/program_knowledge_base.h"

namespace spa {
class QueryProcessingSubsystem {
  public:
    void Use(std::unique_ptr<spa::KnowledgeBase> pkb_ptr);
    void Evaluate(std::string_view query_string, std::list<std::string> &list);

  private:
    std::shared_ptr<spa::KnowledgeBase> pkb_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_QUERY_PROCESSING_SUBSYSTEM_H_
