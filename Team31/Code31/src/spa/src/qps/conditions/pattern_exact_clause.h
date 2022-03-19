#ifndef SPA_SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_EXACT_CLAUSE_H_
#define SPA_SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_EXACT_CLAUSE_H_

#include "pattern_base.h"

namespace spa {
class PatternExactClause : public PatternBase {
    using PatternBase::PatternBase;

  public:
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_EXACT_CLAUSE_H_
