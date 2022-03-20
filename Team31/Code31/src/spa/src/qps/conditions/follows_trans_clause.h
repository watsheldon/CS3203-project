#ifndef SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_TRANS_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_TRANS_CLAUSE_H_

#include "ordered_stmt_stmt_base.h"

namespace spa {
class FollowsTransClause : public OrderedStmtStmtBase {
    using OrderedStmtStmtBase::OrderedStmtStmtBase;
    ResultTable Execute(KnowledgeBase *knowledge_base) const final;
};

}  // namespace spa
#endif  // RC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_TRANS_CLAUSE_H_
