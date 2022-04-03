#ifndef SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_WHILE_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_WHILE_CLAUSE_H_

#include "common/type_convert_helpers.h"
#include "pattern_base.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
class PatternWhileIfClause : public PatternBase {
  protected:
    using PatternBase::PatternBase;
    ResultTable VarWild(KnowledgeBase *pkb, VarName first) const noexcept final;
    ResultTable SynWild(KnowledgeBase *pkb,
                        Synonym *first) const noexcept final;
    ResultTable WildWild(KnowledgeBase *pkb) const noexcept final;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_WHILE_CLAUSE_H_
