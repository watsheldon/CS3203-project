#include "pattern_if_clause.h"

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable PatternIfClause::VarWild(KnowledgeBase *pkb,
                                     VarName first) const noexcept {
    auto result = pkb->GetPattern(StmtType::kIf, first);
    return {zeroth_param_, std::move(result)};
}
ResultTable PatternIfClause::SynWild(KnowledgeBase *pkb,
                                     Synonym *first) const noexcept {
    auto [col_1, col_2] = pkb->GetPatternPairs(StmtType::kIf);
    return {zeroth_param_, std::move(col_1), first, std::move(col_2)};
}
ResultTable PatternIfClause::WildWild(KnowledgeBase *pkb) const noexcept {
    auto result = pkb->GetPattern(StmtType::kIf);
    return {zeroth_param_, std::move(result)};
}
}  // namespace spa
