#include "variable_column.h"

#include <memory>
#include <set>
#include <vector>

namespace spa {

VariableColumn::VariableColumn(Synonym *syn,
                               std::set<int> &&syn_domain) noexcept
        : VariableColumn(syn, std::move(syn_domain), {}) {}
VariableColumn::VariableColumn(Synonym *syn, std::vector<int> &&values) noexcept
        : VariableColumn(syn, std::set<int>(values.begin(), values.end()),
                         std::move(values)) {}
VariableColumn::VariableColumn(Synonym *syn, std::set<int> &&syn_domain,
                               std::vector<int> &&values) noexcept
        : synonym(syn),
          domain(std::move(syn_domain)),
          column(std::move(values)) {}
void VariableColumn::Assign(std::set<int> &&new_domain,
                            std::vector<int> &&new_column) noexcept {
    domain.swap(new_domain);
    column.swap(new_column);
}
}  // namespace spa
