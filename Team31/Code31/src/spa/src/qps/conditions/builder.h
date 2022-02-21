#ifndef SPA_SRC_SPA_SRC_QPS_CONDITIONS_BUILDER_H_
#define SPA_SRC_SPA_SRC_QPS_CONDITIONS_BUILDER_H_

#include <memory>

#include "condition_clause.h"

namespace spa {
class Builder {
  public:
    virtual std::unique_ptr<ConditionClause> build() = 0;
    virtual ~Builder() = 0;
};
inline Builder::~Builder() = default;
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_CONDITIONS_BUILDER_H_
