#ifndef SRC_SPA_SRC_QPS_EVALUATOR_VARIABLE_COLUMN_H_
#define SRC_SPA_SRC_QPS_EVALUATOR_VARIABLE_COLUMN_H_

#include <memory>
#include <set>
#include <vector>

#include "qps/synonym.h"

namespace spa {
struct VariableColumn {
    using Domain = std::set<int>;
    using Column = std::vector<int>;
    VariableColumn(Synonym *syn, Domain &&syn_domain) noexcept;
    VariableColumn(Synonym *syn, std::vector<int> &&values) noexcept;
    Synonym *const synonym;
    Domain domain;
    Column column;
    void Assign(Domain &&new_domain, Column &&new_column) noexcept;

  private:
    VariableColumn(Synonym *syn, Domain &&syn_domain, Column &&values) noexcept;
};

}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_EVALUATOR_VARIABLE_COLUMN_H_
