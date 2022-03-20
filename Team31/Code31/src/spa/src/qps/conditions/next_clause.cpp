#include "next_clause.h"

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable NextClause::Execute(KnowledgeBase *knowledge_base) const {
    return ResultTable{false};
}
}  // namespace spa
