#ifndef SPA_SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_BUILDER_H_
#define SPA_SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_BUILDER_H_

#include "builder.h"
#include "pattern_clause.h"

namespace spa {
class PatternBuilder : public Builder {
  public:
    std::unique_ptr<ConditionClause> build() override;
    ~PatternBuilder() override;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_BUILDER_H_
