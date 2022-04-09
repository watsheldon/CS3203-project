#ifndef SRC_SPA_SRC_QPS_CONDITIONS_USES_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_USES_CLAUSE_H_

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"
#include "uses_modifies_base.h"

namespace spa {
class UsesClause : public UsesModifiesBase {
  public:
    using UsesModifiesBase::UsesModifiesBase;
    ResultTable NumVar(KnowledgeBase *pkb, StmtNo first,
                       VarName second) const noexcept override;
    ResultTable NumSyn(KnowledgeBase *pkb, StmtNo first,
                       Synonym *second) const noexcept override;
    ResultTable NumWild(KnowledgeBase *pkb,
                        StmtNo first) const noexcept override;
    ResultTable SynVar(KnowledgeBase *pkb, Synonym *first,
                       VarName second) const noexcept override;
    ResultTable SynSyn(KnowledgeBase *pkb, Synonym *first,
                       Synonym *second) const noexcept override;
    ResultTable SynWild(KnowledgeBase *pkb,
                        Synonym *first) const noexcept override;
    ResultTable ProcVar(KnowledgeBase *pkb, ProcName first,
                        VarName second) const noexcept override;
    ResultTable ProcSyn(KnowledgeBase *pkb, ProcName first,
                        Synonym *second) const noexcept override;
    ResultTable ProcWild(KnowledgeBase *pkb,
                         ProcName first) const noexcept override;
    [[nodiscard]] int GetPriority() const noexcept final;

  private:
    static constexpr int kPriority = 8;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_USES_CLAUSE_H_
