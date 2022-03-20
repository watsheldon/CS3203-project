#include "affects_clause.h"

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable AffectsClause::Execute(
        KnowledgeBase *knowledge_base) const noexcept {
    return ResultTable{false};
}

}  // namespace spa
