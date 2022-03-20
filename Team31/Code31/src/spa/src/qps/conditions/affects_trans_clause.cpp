#include "affects_trans_clause.h"

namespace spa {
ResultTable AffectsTransClause::Execute(KnowledgeBase *knowledge_base) const {
    return StmtStmtBase::Execute(knowledge_base);
}
}  // namespace spa
