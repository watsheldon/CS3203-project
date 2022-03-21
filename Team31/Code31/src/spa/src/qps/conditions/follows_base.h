#ifndef SPA_SRC_QPS_CONDITIONS_FOLLOWS_BASE_H_
#define SPA_SRC_QPS_CONDITIONS_FOLLOWS_BASE_H_

#include "ordered_stmt_stmt_base.h"

namespace spa {
class FollowsBase : public OrderedStmtStmtBase {
  protected:
    using OrderedStmtStmtBase::OrderedStmtStmtBase;
    ResultTable NumWild(KnowledgeBase *pkb, StmtNo first) const noexcept final;
    ResultTable SynWild(KnowledgeBase *pkb,
                        Synonym *first) const noexcept final;
    ResultTable WildNum(KnowledgeBase *pkb, StmtNo second) const noexcept final;
    ResultTable WildSyn(KnowledgeBase *pkb,
                        Synonym *second) const noexcept final;
    ResultTable WildWild(KnowledgeBase *pkb) const noexcept final;
};
}  // namespace spa

#endif  // SPA_SRC_QPS_CONDITIONS_FOLLOWS_BASE_H_
