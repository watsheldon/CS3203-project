#include "formatter.h"

#include <list>
#include <string>

#include "evaluator/query_evaluator.h"
#include "pkb/knowledge_base.h"
#include "synonym.h"

namespace spa {
Formatter::Formatter(QueryEvaluator::ResultsView results_view) noexcept
        : synonym_domains_(results_view.synonym_domain),
          vartable_map_(results_view.vartable_map) {}
void Formatter::Use(KnowledgeBase *pkb) noexcept { pkb_ = pkb; }
void Formatter::OutputResults(std::list<std::string> &results,
                              const Synonym *const selected) {
    auto domain = synonym_domains_.find(selected);
    auto stmt_nos = domain == synonym_domains_.end()
                            ? pkb_->GetAllEntityIndices(selected->type)
                            : std::vector<int>(domain->second.begin(),
                                               domain->second.end());
    pkb_->ToName(selected->type, stmt_nos, results);
}
}  // namespace spa
