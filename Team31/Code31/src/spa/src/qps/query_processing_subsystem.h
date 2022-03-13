#ifndef SRC_SPA_SRC_QPS_QUERY_PROCESSING_SUBSYSTEM_H_
#define SRC_SPA_SRC_QPS_QUERY_PROCESSING_SUBSYSTEM_H_

#include <list>
#include <memory>
#include <string_view>

#include "PQL_validator.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/query_evaluator.h"

namespace spa {
class QueryProcessingSubsystem {
  public:
    void Use(std::unique_ptr<spa::KnowledgeBase> pkb_ptr) noexcept;
    void Evaluate(std::string_view query_string,
                  std::list<std::string> &list) noexcept;

  private:
    std::unique_ptr<QueryEvaluator> evaluator_;
    PQLValidator validator_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_QUERY_PROCESSING_SUBSYSTEM_H_
