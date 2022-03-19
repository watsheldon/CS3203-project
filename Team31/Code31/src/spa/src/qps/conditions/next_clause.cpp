#include "next_clause.h"

namespace spa {
ResultTable NextClause::Execute(KnowledgeBase *knowledge_base) const {
    return StmtStmtBase::Execute(knowledge_base);
}
}  // namespace spa