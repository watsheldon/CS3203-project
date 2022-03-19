#include "affects_trans_clause.h"

spa::ResultTable spa::AffectsTransClause::Execute(
        spa::KnowledgeBase *knowledge_base) const {
    return StmtStmtBase::Execute(knowledge_base);
}