#ifndef SRC_SPA_SRC_QPS_CONDITIONS_PARENT_TRANS_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_PARENT_TRANS_CLAUSE_H_

#include "ordered_stmt_stmt_base.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
class ParentTransClause : public OrderedStmtStmtBase {
    using OrderedStmtStmtBase::OrderedStmtStmtBase;

  public:
    ResultTable Execute(KnowledgeBase *knowledge_base) const final;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_PARENT_TRANS_CLAUSE_H_
