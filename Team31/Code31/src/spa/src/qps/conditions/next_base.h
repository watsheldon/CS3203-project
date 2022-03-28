#ifndef SRC_SPA_SRC_QPS_CONDITIONS_NEXT_BASE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_NEXT_BASE_H_

#include "stmt_stmt_base.h"

namespace spa {
class NextBase : public StmtStmtBase {
  protected:
    using StmtStmtBase::StmtStmtBase;
    ResultTable NumWild(KnowledgeBase *pkb, StmtNo first) const noexcept final;
    ResultTable WildNum(KnowledgeBase *pkb, StmtNo second) const noexcept final;
    ResultTable WildWild(KnowledgeBase *pkb) const noexcept final;
    ResultTable SynWild(KnowledgeBase *pkb,
                        Synonym *first) const noexcept final;
    ResultTable WildSyn(KnowledgeBase *pkb,
                        Synonym *second) const noexcept final;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_CONDITIONS_NEXT_BASE_H_
