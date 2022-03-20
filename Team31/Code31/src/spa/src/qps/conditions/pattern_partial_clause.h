#ifndef SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_PARTIAL_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_PARTIAL_CLAUSE_H_

#include "pattern_base.h"

namespace spa {
class PatternPartialClause : public PatternBase {
    using PatternBase::PatternBase;

  public:
    ResultTable Execute(KnowledgeBase *knowledge_base) const final;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_PARTIAL_CLAUSE_H_
