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
void Formatter::OutputResults(
        std::list<std::string> &results,
        std::vector<SynonymWithAttr> &selected) const noexcept {
    if (selected.front().synonym == nullptr) {
        results.emplace_back(results_valid_ ? kTrue : kFalse);
        return;
    }
    if (!results_valid_) return;
    auto sel = selected.front().synonym;
    auto domain = synonym_domains_.find(sel);
    Synonym::Type syn_type = sel->type;
    auto indices = domain == synonym_domains_.end()
                           ? GetAllIndices(syn_type)
                           : std::vector<int>(domain->second.begin(),
                                              domain->second.end());
    QueryEntityType element_type = ToNamedElementType(syn_type);
    if (selected.front().attribute == Attribute::kNone ||
        element_type != QueryEntityType::kStmt ||
        syn_type == Synonym::kStmtAny ||
        selected.front().attribute == Attribute::kStmtNum) {
        pkb_->ToName(element_type, indices, results);
    } else {
        pkb_->ToAttr(static_cast<StmtType>(syn_type), indices, results);
    }
}
std::vector<int> Formatter::GetAllIndices(Synonym::Type type) const noexcept {
    QueryEntityType element_type = ToNamedElementType(type);
    if (element_type == QueryEntityType::kStmt) {
        return pkb_->GetAllEntityIndices(static_cast<StmtType>(type));
    }
    return pkb_->GetAllEntityIndices(element_type);
}
}  // namespace spa
