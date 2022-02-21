#ifndef SPA_SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_BUILDER_H_
#define SPA_SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_BUILDER_H_

#include <memory>

#include "builder.h"
#include "follows_clause.h"
namespace spa {
class FollowsBuilder : public Builder {
  public:
    std::unique_ptr<ConditionClause> build() override;
    ~FollowsBuilder() override;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_BUILDER_H_
