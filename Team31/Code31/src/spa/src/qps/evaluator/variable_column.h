#ifndef SRC_SPA_SRC_QPS_EVALUATOR_VARIABLE_COLUMN_H_
#define SRC_SPA_SRC_QPS_EVALUATOR_VARIABLE_COLUMN_H_

#include <memory>
#include <set>
#include <vector>

#include "qps/synonym.h"

namespace spa {
struct VariableColumn {
    VariableColumn(Synonym *syn, std::set<int> &&syn_domain);
    VariableColumn(Synonym *syn, std::vector<int> &&values);
    Synonym *const synonym;
    std::set<int> domain;
    std::vector<int> column;
    void Clear();
    void Update(std::set<int> &&new_domain, std::vector<int> &&new_column);

  private:
    VariableColumn(Synonym *syn, std::set<int> &&syn_domain,
                   std::vector<int> &&values);
};

}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_EVALUATOR_VARIABLE_COLUMN_H_
