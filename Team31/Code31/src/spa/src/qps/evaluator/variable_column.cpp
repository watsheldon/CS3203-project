#include "variable_column.h"

#include <memory>
#include <set>
#include <vector>

namespace spa {

VariableColumn::VariableColumn(Synonym *syn, std::set<int> &&syn_domain)
        : VariableColumn(syn, std::move(syn_domain), {}) {}
VariableColumn::VariableColumn(Synonym *syn, std::vector<int> &&values)
        : VariableColumn(syn, std::set<int>(values.begin(), values.end()),
                         std::move(values)) {}
VariableColumn::VariableColumn(Synonym *syn, std::set<int> &&syn_domain,
                               std::vector<int> &&values)
        : synonym(syn),
          domain(std::move(syn_domain)),
          column(std::move(values)) {}
void VariableColumn::Clear() {
    domain.clear();
    column.clear();
}
void VariableColumn::Update(std::set<int> &&new_domain,
                            std::vector<int> &&new_column) {
    domain.swap(new_domain);
    column.swap(new_column);
}
}  // namespace spa
