#include "next_trans_clause.h"

spa::ResultTable spa::NextTransClause::Execute(
        spa::KnowledgeBase *knowledge_base) const {
    return StmtStmtBase::Execute(knowledge_base);
}
