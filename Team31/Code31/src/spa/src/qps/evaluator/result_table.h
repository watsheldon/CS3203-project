#ifndef SRC_SPA_SRC_QPS_EVALUATOR_RESULT_TABLE_H_
#define SRC_SPA_SRC_QPS_EVALUATOR_RESULT_TABLE_H_

#include <memory>
#include <set>
#include <vector>

#include "common/aliases.h"
#include "qps/synonym.h"
#include "variable_column.h"

namespace spa {

class ResultTable {
  public:
    using Domain = VariableColumn::Domain;
    using Column = VariableColumn::Column;
    using DomainPair = Pair<Domain>;
    using SynonymPair = Pair<const Synonym *>;
    enum ResultType { kBool, kSingle, kDouble };

    static Domain Intersect(const Domain &first, const Domain &second);
    explicit ResultTable(bool found_result = false);
    ResultTable(Synonym *synonym, Domain &&domain);
    ResultTable(Synonym *synonym_1, std::vector<int> &&column_1,
                Synonym *synonym_2, std::vector<int> &&column_2);

    DomainPair Update(const ResultTable &other);
    const Domain &Update(const Synonym *syn_a, const Domain &domain_a);
    DomainPair Update(const Synonym *syn_a, const Domain &domain_a,
                      const Synonym *syn_b, const Domain &domain_b);
    [[nodiscard]] const VariableColumn *GetFirstColumn() const;
    [[nodiscard]] const VariableColumn *GetSecondColumn() const;
    [[nodiscard]] SynonymPair GetSynonyms() const;

    const bool has_result;
    const ResultType type;

  private:
    using Bitmap = Vec2D<bool>;
    std::unique_ptr<VariableColumn> first_;
    std::unique_ptr<VariableColumn> second_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_EVALUATOR_RESULT_TABLE_H_
