#ifndef SRC_SPA_SRC_QPS_FORMATTER_H_
#define SRC_SPA_SRC_QPS_FORMATTER_H_

#include <list>
#include <string>

#include "evaluator/query_evaluator.h"
#include "pkb/knowledge_base.h"

namespace spa {
class Formatter {
  public:
    explicit Formatter(QueryEvaluator::ResultsView results_view) noexcept;
    void Use(KnowledgeBase* pkb) noexcept;
    void OutputResults(std::list<std::string>& results,
                       const Synonym* selected);

  private:
    KnowledgeBase* pkb_{};
    const QueryEvaluator::SynonymDomains& synonym_domains_;
    const QueryEvaluator::VarTableMap& vartable_map_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_FORMATTER_H_
