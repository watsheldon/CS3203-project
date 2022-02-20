#ifndef SPA_SRC_SPA_SRC_QPS_CONDITIONS_USES_MODIFIES_BUILDER_H_
#define SPA_SRC_SPA_SRC_QPS_CONDITIONS_USES_MODIFIES_BUILDER_H_

#include "uses_modifies_clause.h"
namespace spa {
class ParentFollowsBuilder {
  public:
    UsesModifiesClause build();
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_CONDITIONS_USES_MODIFIES_BUILDER_H_
