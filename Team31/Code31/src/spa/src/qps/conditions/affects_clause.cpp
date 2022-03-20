#include "affects_clause.h"

namespace spa {
ResultTable AffectsClause::Execute(KnowledgeBase *knowledge_base) const {
    return StmtStmtBase::Execute(knowledge_base);
}

}  // namespace spa
