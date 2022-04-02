#ifndef SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_PARTIAL_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_PARTIAL_CLAUSE_H_

#include "pattern_expr_base.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
class PatternPartialClause : public PatternExprBase {
  public:
    using PatternExprBase::PatternExprBase;

  protected:
    ResultTable VarExpr(KnowledgeBase *pkb, VarName first,
                        Expression second) const noexcept override;
    ResultTable SynExpr(KnowledgeBase *pkb, Synonym *first,
                        Expression second) const noexcept override;
    ResultTable WildExpr(KnowledgeBase *pkb,
                         Expression second) const noexcept override;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_PARTIAL_CLAUSE_H_
