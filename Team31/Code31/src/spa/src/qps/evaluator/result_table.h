#ifndef SRC_SPA_SRC_QPS_EVALUATOR_RESULT_TABLE_H_
#define SRC_SPA_SRC_QPS_EVALUATOR_RESULT_TABLE_H_

#include <memory>
#include <set>
#include <vector>

#include "common/aliases.h"
#include "qps/aliases.h"
#include "qps/synonym.h"
#include "variable_column.h"

namespace spa {

class ResultTable {
  public:
    using DomainPair = PairOf<Domain>;
    using SynonymPair = PairOf<const Synonym *>;
    enum ResultType { kBool, kSingle, kDouble };

    static Domain Intersect(const Domain &first, const Domain &second) noexcept;
    explicit ResultTable(bool found_result = false) noexcept;
    ResultTable(Synonym *synonym, Domain &&domain) noexcept;
    ResultTable(Synonym *synonym_1, std::vector<int> &&column_1,
                Synonym *synonym_2, std::vector<int> &&column_2) noexcept;

    DomainPair Update(const ResultTable &other) noexcept;
    const Domain &Update(const Synonym *syn_a, const Domain &domain_a) noexcept;
    DomainPair Update(const Synonym *syn_a, const Domain &domain_a,
                      const Synonym *syn_b, const Domain &domain_b) noexcept;
    [[nodiscard]] const VariableColumn *GetFirstColumn() const noexcept;
    [[nodiscard]] const VariableColumn *GetSecondColumn() const noexcept;
    [[nodiscard]] SynonymPair GetSynonyms() const noexcept;

    const bool has_result;
    const ResultType type;

  private:
    using Bitmap = Vec2D<bool>;
    std::unique_ptr<VariableColumn> first_;
    std::unique_ptr<VariableColumn> second_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_EVALUATOR_RESULT_TABLE_H_
