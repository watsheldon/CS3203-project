#ifndef SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_EXACT_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_EXACT_CLAUSE_H_

#include "pattern_base.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
class PatternExactClause : public PatternBase {
  public:
    using PatternBase::PatternBase;

  protected:
    ResultTable VarExpr(KnowledgeBase *pkb, VarName first,
                        Expression second) const noexcept final;
    ResultTable SynExpr(KnowledgeBase *pkb, Synonym *first,
                        Expression second) const noexcept final;
    ResultTable WildExpr(KnowledgeBase *pkb,
                         Expression second) const noexcept final;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_EXACT_CLAUSE_H_
