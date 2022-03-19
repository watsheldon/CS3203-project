#ifndef SPA_SRC_SPA_SRC_QPS_CONDITIONS_PARENT_TRANS_CLAUSE_H_
#define SPA_SRC_SPA_SRC_QPS_CONDITIONS_PARENT_TRANS_CLAUSE_H_

#include "stmt_stmt_base.h"

namespace spa {
class ParentTransClause : public StmtStmtBase {
    using StmtStmtBase::StmtStmtBase;

  public:
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_CONDITIONS_PARENT_TRANS_CLAUSE_H_
