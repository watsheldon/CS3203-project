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
    VariableColumn(Synonym *syn, Domain &&syn_domain);
    VariableColumn(Synonym *syn, std::vector<int> &&values);
    Synonym *const synonym;
    Domain domain;
    Column column;
    void Assign(Domain &&new_domain, Column &&new_column);

  private:
    VariableColumn(Synonym *syn, Domain &&syn_domain, Column &&values);
};

}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_EVALUATOR_VARIABLE_COLUMN_H_
