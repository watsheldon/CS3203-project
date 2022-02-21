#ifndef SPA_SRC_SPA_SRC_QPS_CONDITIONS_PARENT_BUILDER_H_
#define SPA_SRC_SPA_SRC_QPS_CONDITIONS_PARENT_BUILDER_H_

#include "builder.h"
#include "parent_clause.h"
namespace spa {
class ParentBuilder : public Builder {
  public:
    std::unique_ptr<ConditionClause> build() override;
    ~ParentBuilder() override;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_CONDITIONS_PARENT_BUILDER_H_
