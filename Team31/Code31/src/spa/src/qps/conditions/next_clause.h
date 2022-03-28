#ifndef SRC_SPA_SRC_QPS_CONDITIONS_NEXT_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_NEXT_CLAUSE_H_

#include "next_base.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
class NextClause : public NextBase {
  public:
    using NextBase::NextBase;

  protected:
    ResultTable NumNum(KnowledgeBase *pkb, StmtNo first,
                       StmtNo second) const noexcept final;
    ResultTable NumSyn(KnowledgeBase *pkb, StmtNo first,
                       Synonym *second) const noexcept final;
    ResultTable SynNum(KnowledgeBase *pkb, Synonym *first,
                       StmtNo second) const noexcept final;
    ResultTable SynSyn(KnowledgeBase *pkb, Synonym *first,
                       Synonym *second) const noexcept final;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_CONDITIONS_NEXT_CLAUSE_H_
