#ifndef SRC_SPA_SRC_QPS_CONDITIONS_NEXT_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_NEXT_CLAUSE_H_

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"
#include "stmt_stmt_base.h"

namespace spa {
class NextClause : public StmtStmtBase {
    using StmtStmtBase::StmtStmtBase;
    ResultTable Execute(KnowledgeBase *knowledge_base) const final;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_CONDITIONS_NEXT_CLAUSE_H_
