#ifndef SRC_SPA_SRC_QPS_FORMATTER_H_
#define SRC_SPA_SRC_QPS_FORMATTER_H_

#include <list>
#include <string>
#include <string_view>

#include "common/entity_type_enum.h"
#include "evaluator/query_evaluator.h"
#include "pkb/knowledge_base.h"
#include "synonym_with_attr.h"

namespace {
using namespace std::literals::string_view_literals;
}

namespace spa {
class Formatter {
  public:
    explicit Formatter(QueryEvaluator::ResultsView results_view) noexcept;
    void Use(KnowledgeBase* pkb) noexcept;
    void OutputResults(std::list<std::string>& results,
                       std::vector<SynonymWithAttr>& selected) const noexcept;
    static inline void OutputInvalid(
            std::list<std::string>& results,
            std::vector<SynonymWithAttr>& selected) noexcept {
        if (selected.size() == 1 && selected.front().synonym == nullptr)
            results.emplace_back(kFalse);
    }

  private:
    static constexpr std::string_view kTrue = "TRUE"sv;
    static constexpr std::string_view kFalse = "FALSE"sv;
    static constexpr QueryEntityType ToNamedElementType(
            Synonym::Type syn_type) noexcept {
        assert(syn_type != Synonym::kNone);
        return syn_type >= Synonym::kProc
                       ? QueryEntityType{static_cast<int>(syn_type) -
                                         static_cast<int>(Synonym::kProc)}
                       : QueryEntityType::kStmt;
    }
    KnowledgeBase* pkb_{};
    const bool& results_valid_;
    const QueryEvaluator::SynonymDomains& synonym_domains_;
    const QueryEvaluator::VarTableMap& vartable_map_;
    [[nodiscard]] std::vector<int> GetAllIndices(
            Synonym::Type type) const noexcept;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_FORMATTER_H_
