#ifndef SRC_SPA_SRC_QPS_EVALUATOR_RESULT_TABLE_H_
#define SRC_SPA_SRC_QPS_EVALUATOR_RESULT_TABLE_H_

#include <memory>
#include <set>
#include <vector>

#include "qps/synonym.h"
#include "variable_column.h"

namespace spa {
class ResultTable {
  public:
    explicit ResultTable(bool found_result = false);
    ResultTable(Synonym *synonym, std::set<int> &&domain);
    ResultTable(Synonym *synonym_1, std::vector<int> &&column_1,
                Synonym *synonym_2, std::vector<int> &&column_2);
    const VariableColumn *Update(const VariableColumn *);
    void Update(const ResultTable &other);

  private:
    using Bitmap = std::vector<std::vector<bool>>;
    bool has_result_;
    std::unique_ptr<VariableColumn> first_;
    std::unique_ptr<VariableColumn> second_;

    static std::set<int> Intersect(const std::set<int> &first,
                                   const std::set<int> &second);
    void Clear();
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_EVALUATOR_RESULT_TABLE_H_
