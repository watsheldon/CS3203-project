#ifndef SPA_SRC_SPA_SRC_QPS_CONDITIONS_USES_BUILDER_H_
#define SPA_SRC_SPA_SRC_QPS_CONDITIONS_USES_BUILDER_H_

#include "builder.h"
#include "uses_clause.h"

namespace spa {
class UsesBuilder : public Builder {
  public:
    std::unique_ptr<ConditionClause> build() override;
    ~UsesBuilder() override;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_CONDITIONS_USES_BUILDER_H_
