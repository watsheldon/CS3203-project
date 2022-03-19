#ifndef SPA_SRC_SPA_SRC_QPS_CONDITIONS_AFFECTS_TRANS_CLAUSE_H_
#define SPA_SRC_SPA_SRC_QPS_CONDITIONS_AFFECTS_TRANS_CLAUSE_H_

#include "stmt_stmt_base.h"

namespace spa {
class AffectsTransClause : public StmtStmtBase {
    using StmtStmtBase::StmtStmtBase;
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_CONDITIONS_AFFECTS_TRANS_CLAUSE_H_