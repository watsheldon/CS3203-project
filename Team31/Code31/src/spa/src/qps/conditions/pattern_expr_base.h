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
    using SecondParam = std::variant<Wildcard, Expression>;
    PatternExprBase(Synonym *syn, VarName first,
                    std::vector<QueryToken> &&second) noexcept;  // IdentExpr
    PatternExprBase(Synonym *syn, Synonym *first,
                    std::vector<QueryToken> &&second) noexcept;  // SynExpr
    PatternExprBase(Synonym *syn,
                    std::vector<QueryToken> &&second) noexcept;  // WildExpr
    ResultTable Execute(KnowledgeBase *pkb) const noexcept final;

  protected:
    using PatternBase::PatternBase;
    SecondParam second_param_;
    virtual ResultTable VarExpr(KnowledgeBase *pkb, VarName first,
                                Expression second) const noexcept = 0;
    virtual ResultTable SynExpr(KnowledgeBase *pkb, Synonym *first,
                                Expression second) const noexcept = 0;
    virtual ResultTable WildExpr(KnowledgeBase *pkb,
                                 Expression second) const noexcept = 0;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
