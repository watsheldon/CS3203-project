#include "pattern_exact_clause.h"

#include <utility>

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable PatternExactClause::VarExpr(KnowledgeBase *pkb, VarName first,
                                        Expression second) const noexcept {
    auto result = pkb->GetPattern(first, second, false);
    return {zeroth_param_, std::move(result)};
}
ResultTable PatternExactClause::SynExpr(KnowledgeBase *pkb, Synonym *first,
                                        Expression second) const noexcept {
    auto [col_1, col_2] = pkb->GetPatternPair(second, false);
    return {zeroth_param_, std::move(col_1), first, std::move(col_2)};
}
ResultTable PatternExactClause::WildExpr(KnowledgeBase *pkb,
                                         Expression second) const noexcept {
    auto result = pkb->GetPattern(second, false);
    return {zeroth_param_, std::move(result)};
}
}  // namespace spa
