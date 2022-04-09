#ifndef SRC_SPA_SRC_QPS_CONDITIONS_CALLS_TRANS_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_CALLS_TRANS_CLAUSE_H_

#include "calls_base.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
class CallsTransClause : public CallsBase {
  public:
    using CallsBase::CallsBase;

  protected:
    ResultTable SynSyn(KnowledgeBase *pkb, Synonym *first,
                       Synonym *second) const noexcept final;
    ResultTable SynProc(KnowledgeBase *pkb, Synonym *first,
                        ProcName second) const noexcept final;
    ResultTable ProcSyn(KnowledgeBase *pkb, ProcName first,
                        Synonym *second) const noexcept final;
    ResultTable ProcProc(KnowledgeBase *pkb, ProcName first,
                         ProcName second) const noexcept final;
    [[nodiscard]] int GetPriority() const noexcept override;

  private:
    static constexpr int kPriority = 6;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_CONDITIONS_CALLS_TRANS_CLAUSE_H_
