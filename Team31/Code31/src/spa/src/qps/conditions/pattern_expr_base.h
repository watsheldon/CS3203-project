#ifndef SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_

#include <array>
#include <vector>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "pattern_base.h"
#include "qps/evaluator/result_table.h"
#include "qps/query_token.h"

namespace spa {
class PatternExprBase : public PatternBase {
  public:
    ResultTable Execute(KnowledgeBase *pkb) const noexcept final;

  protected:
    using PatternBase::PatternBase;
    virtual ResultTable VarExpr(KnowledgeBase *pkb, VarName first,
                                Expression second) const noexcept = 0;
    virtual ResultTable SynExpr(KnowledgeBase *pkb, Synonym *first,
                                Expression second) const noexcept = 0;
    virtual ResultTable WildExpr(KnowledgeBase *pkb,
                                 Expression second) const noexcept = 0;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
