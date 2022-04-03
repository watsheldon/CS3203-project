#include "pattern_while_if_clause.h"

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable PatternWhileIfClause::VarWild(KnowledgeBase *pkb,
                                          VarName first) const noexcept {
    auto result = pkb->GetPattern(SynToPkbType(zeroth_param_), first);
    return {zeroth_param_, std::move(result)};
}
ResultTable PatternWhileIfClause::SynWild(KnowledgeBase *pkb,
                                          Synonym *first) const noexcept {
    auto [col_1, col_2] = pkb->GetPatternPairs(SynToPkbType(zeroth_param_));
    return {zeroth_param_, std::move(col_1), first, std::move(col_2)};
}
ResultTable PatternWhileIfClause::WildWild(KnowledgeBase *pkb) const noexcept {
    auto result = pkb->GetPattern(SynToPkbType(zeroth_param_));
    return {zeroth_param_, std::move(result)};
}
}  // namespace spa
