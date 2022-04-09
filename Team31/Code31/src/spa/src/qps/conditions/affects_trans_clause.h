#ifndef SRC_SPA_SRC_QPS_CONDITIONS_AFFECTS_TRANS_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_AFFECTS_TRANS_CLAUSE_H_

#include "affects_base.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
class AffectsTransClause : public AffectsBase {
  public:
    using AffectsBase::AffectsBase;

  protected:
    ResultTable NumNum(KnowledgeBase *pkb, StmtNo first,
                       StmtNo second) const noexcept final;
    ResultTable NumSyn(KnowledgeBase *pkb, StmtNo first,
                       Synonym *second) const noexcept final;
    ResultTable SynNum(KnowledgeBase *pkb, Synonym *first,
                       StmtNo second) const noexcept final;
    ResultTable SynSyn(KnowledgeBase *pkb, Synonym *first,
                       Synonym *second) const noexcept final;
    [[nodiscard]] int GetPriority() const noexcept final;

  private:
    static constexpr int kPriority = 17;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_AFFECTS_TRANS_CLAUSE_H_
