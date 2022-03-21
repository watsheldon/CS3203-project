#ifndef SRC_SPA_SRC_QPS_FORMATTER_H_
#define SRC_SPA_SRC_QPS_FORMATTER_H_

#include <list>
#include <string>
#include <string_view>

#include "evaluator/query_evaluator.h"
#include "pkb/knowledge_base.h"

namespace {
using namespace std::literals::string_view_literals;
}

namespace spa {
class Formatter {
  public:
    explicit Formatter(QueryEvaluator::ResultsView results_view) noexcept;
    void Use(KnowledgeBase* pkb) noexcept;
    void OutputResults(std::list<std::string>& results,
                       std::vector<const Synonym*>& selected);
    static constexpr void OutputInvalid(
            std::list<std::string>& results,
            const std::vector<const Synonym*>& selected) {
        if (selected.empty()) results.emplace_back(kFalse);
    }

  private:
    static constexpr std::string_view kTrue = "TRUE"sv;
    static constexpr std::string_view kFalse = "FALSE"sv;
    KnowledgeBase* pkb_{};
    const bool& results_valid_;
    const QueryEvaluator::SynonymDomains& synonym_domains_;
    const QueryEvaluator::VarTableMap& vartable_map_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_FORMATTER_H_
