#include "affects_clause.h"

spa::ResultTable spa::AffectsClause::Execute(
        spa::KnowledgeBase *knowledge_base) const {
    return StmtStmtBase::Execute(knowledge_base);
}
