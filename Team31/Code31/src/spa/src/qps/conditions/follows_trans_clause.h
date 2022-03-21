#ifndef SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_TRANS_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_TRANS_CLAUSE_H_

#include "follows_base.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
class FollowsTransClause : public FollowsBase {
  public:
    using FollowsBase::FollowsBase;

  protected:
    ResultTable NumNum(KnowledgeBase *pkb, StmtNo first,
                       StmtNo second) const noexcept override;
    ResultTable NumSyn(KnowledgeBase *pkb, StmtNo first,
                       Synonym *second) const noexcept override;
    ResultTable SynNum(KnowledgeBase *pkb, Synonym *first,
                       StmtNo second) const noexcept override;
    ResultTable SynSyn(KnowledgeBase *pkb, Synonym *first,
                       Synonym *second) const noexcept override;
};

}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_TRANS_CLAUSE_H_
