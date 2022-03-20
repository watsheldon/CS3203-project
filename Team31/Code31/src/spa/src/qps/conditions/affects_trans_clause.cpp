#include "affects_trans_clause.h"

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable AffectsTransClause::Execute(KnowledgeBase *knowledge_base) const {
    return StmtStmtBase::Execute(knowledge_base);
}
}  // namespace spa
