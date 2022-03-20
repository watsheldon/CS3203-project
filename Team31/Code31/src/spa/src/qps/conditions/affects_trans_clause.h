#ifndef SRC_SPA_SRC_QPS_CONDITIONS_AFFECTS_TRANS_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_AFFECTS_TRANS_CLAUSE_H_

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"
#include "stmt_stmt_base.h"

namespace spa {
class AffectsTransClause : public StmtStmtBase {
  public:
    using StmtStmtBase::StmtStmtBase;
    ResultTable Execute(KnowledgeBase *knowledge_base) const noexcept final;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_AFFECTS_TRANS_CLAUSE_H_
