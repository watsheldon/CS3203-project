#include "formatter.h"

#include <list>
#include <string>

#include "evaluator/query_evaluator.h"
#include "pkb/knowledge_base.h"
#include "synonym.h"

namespace spa {
Formatter::Formatter(QueryEvaluator::ResultsView results_view) noexcept
        : results_valid_(results_view.has_valid_result),
          synonym_domains_(results_view.synonym_domain),
          vartable_map_(results_view.vartable_map) {}
void Formatter::Use(KnowledgeBase *pkb) noexcept { pkb_ = pkb; }
void Formatter::OutputResults(std::list<std::string> &results,
                              std::vector<const Synonym *> &selected) {
    if (selected.empty()) {
        results.emplace_back(results_valid_ ? kTrue : kFalse);
        return;
    }
    if (!results_valid_) return;
    auto sel = selected.front();
    auto domain = synonym_domains_.find(sel);
    auto stmt_nos = domain == synonym_domains_.end()
                            ? pkb_->GetAllEntityIndices(sel->type)
                            : std::vector<int>(domain->second.begin(),
                                               domain->second.end());
    pkb_->ToName(sel->type, stmt_nos, results);
}
}  // namespace spa
