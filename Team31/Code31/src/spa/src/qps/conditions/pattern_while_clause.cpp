#include "pattern_while_clause.h"

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable PatternWhileClause::VarWild(KnowledgeBase *pkb,
                                        VarName first) const noexcept {
    auto result = pkb->GetPattern(StmtType::kWhile, first);
    return {zeroth_param_, std::move(result)};
}
ResultTable PatternWhileClause::SynWild(KnowledgeBase *pkb,
                                        Synonym *first) const noexcept {
    auto [col_1, col_2] = pkb->GetPatternPairs(StmtType::kWhile);
    return {zeroth_param_, std::move(col_1), first, std::move(col_2)};
}
ResultTable PatternWhileClause::WildWild(KnowledgeBase *pkb) const noexcept {
    auto result = pkb->GetPattern(StmtType::kWhile);
    return {zeroth_param_, std::move(result)};
}
}  // namespace spa
