#include "pattern_assign_partial.h"

#include <utility>

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable PatternPartialClause::VarExpr(KnowledgeBase *pkb, VarName first,
                                          Expression second) const noexcept {
    auto result = pkb->GetPatternP(first, second);
    return {zeroth_param_, std::move(result)};
}
ResultTable PatternPartialClause::SynExpr(KnowledgeBase *pkb, Synonym *first,
                                          Expression second) const noexcept {
    auto [col_1, col_2] = pkb->GetPatternPairP(second);
    return {zeroth_param_, std::move(col_1), first, std::move(col_2)};
}
ResultTable PatternPartialClause::WildExpr(KnowledgeBase *pkb,
                                           Expression second) const noexcept {
    auto result = pkb->GetPatternP(second);
    return {zeroth_param_, std::move(result)};
}
}  // namespace spa
