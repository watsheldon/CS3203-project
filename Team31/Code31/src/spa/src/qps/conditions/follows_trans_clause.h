#ifndef SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_TRANS_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_TRANS_CLAUSE_H_

#include "ordered_stmt_stmt_base.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
class FollowsTransClause : public OrderedStmtStmtBase {
    using OrderedStmtStmtBase::OrderedStmtStmtBase;
    ResultTable Execute(KnowledgeBase *knowledge_base) const final;
};

}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_TRANS_CLAUSE_H_
