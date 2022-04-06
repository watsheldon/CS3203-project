#ifndef SRC_SPA_SRC_QPS_FORMATTER_H_
#define SRC_SPA_SRC_QPS_FORMATTER_H_

#include <list>
#include <string>
#include <string_view>

#include "common/aliases.h"
#include "common/bitvec2d.h"
#include "common/entity_type_enum.h"
#include "evaluator/query_evaluator.h"
#include "evaluator/result_table.h"
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
                       const std::vector<SynonymWithAttr>& selected) noexcept;
    static inline void OutputInvalid(
            std::list<std::string>& results,
            const std::vector<SynonymWithAttr>& selected) noexcept {
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
    const QueryEvaluator::ResultTables& result_tables_;
    const QueryEvaluator::VarTableMap& vartable_map_;
    std::vector<const Synonym*> constrained_syns_;
    std::vector<const Synonym*> free_syns_;
    std::vector<VecUniquePtr<BitVec2D>> expanded_results_;
    std::map<const Synonym*, std::vector<int>> output_columns_;
    std::map<const Synonym*, std::vector<int>> free_values_;

    [[nodiscard]] std::vector<int> GetAllIndices(
            Synonym::Type type) const noexcept;
    void OutputSingle(std::list<std::string>& results,
                      const SynonymWithAttr& syn_attr) noexcept;
    void OutputTuple(std::list<std::string>& results,
                     const std::vector<SynonymWithAttr>& selected) noexcept;
    void Reset() noexcept;
    void ExtractSynonyms(const std::vector<SynonymWithAttr>& selected) noexcept;
    void ExpandConstrainedVarTables() noexcept;
    void ExpandResultTable(PairOf<size_t> positions, int table_idx) noexcept;
    void ExpandOutputColumns(std::vector<int>& row) noexcept;
    void PopulateFreeSyns() noexcept;
    void AddToOutput(std::vector<int>& row) noexcept;
    void Export(std::list<std::string>& results,
                const std::vector<SynonymWithAttr>& selected) const noexcept;
    void ExportColumn(std::list<std::string>& results,
                      const SynonymWithAttr& syn_attr) const noexcept;
    static void Append(std::list<std::string>& results,
                       const std::vector<int>& indices) noexcept;
    static void Append(std::list<std::string>& results,
                       const std::vector<std::string_view>& names);
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_FORMATTER_H_
