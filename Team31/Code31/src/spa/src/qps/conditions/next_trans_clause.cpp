#include "next_trans_clause.h"

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

spa::ResultTable spa::NextTransClause::Execute(
        spa::KnowledgeBase *knowledge_base) const {
    return StmtStmtBase::Execute(knowledge_base);
}
