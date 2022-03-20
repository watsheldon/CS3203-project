#include "pattern_exact_clause.h"

namespace spa {
ResultTable PatternExactClause::Execute(KnowledgeBase *knowledge_base) const {
    switch (type_) {
        case Type::kIdentExpr: {
            auto result = knowledge_base->GetPattern(first_ident_, second_expr_,
                                                     false);
            return {assign_, std::move(result)};
        }
        case Type::kSynExpr: {
            auto [col_1, col_2] =
                    knowledge_base->GetPatternPair(second_expr_, false);
            return {assign_, std::move(col_1), first_syn_, std::move(col_2)};
        }
        case Type::kWildExpr: {
            auto result = knowledge_base->GetPattern(second_expr_, false);
            return {assign_, std::move(result)};
        }
        default:
            assert(false);
    }
}
}  // namespace spa
