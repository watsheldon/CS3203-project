#ifndef SPA_SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_BUILDER_H_
#define SPA_SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_BUILDER_H_

#include <memory>

#include "builder.h"
#include "modifies_clause.h"
namespace spa {
class ModifiesBuilder : public Builder {
  public:
    std::unique_ptr<ConditionClause> build() override;
    ~ModifiesBuilder() override;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_BUILDER_H_
