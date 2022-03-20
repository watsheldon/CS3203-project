#include "pattern_partial_clause.h"

#include <cassert>
#include <utility>

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable PatternPartialClause::Execute(KnowledgeBase *knowledge_base) const {
    switch (type_) {
        case Type::kIdentExpr: {
            auto result = knowledge_base->GetPattern(first_ident_, second_expr_,
                                                     true);
            return {assign_, std::move(result)};
        }
        case Type::kIdentWild: {
            auto result = knowledge_base->GetPattern(first_ident_);
            return {assign_, std::move(result)};
        }
        case Type::kSynExpr: {
            auto [col_1, col_2] =
                    knowledge_base->GetPatternPair(second_expr_, true);
            return {assign_, std::move(col_1), first_syn_, std::move(col_2)};
        }
        case Type::kSynWild: {
            auto [col_1, col_2] = knowledge_base->GetPatternPair();
            return {assign_, std::move(col_1), first_syn_, std::move(col_2)};
        }
        case Type::kWildExpr: {
            auto result = knowledge_base->GetPattern(second_expr_, true);
            return {assign_, std::move(result)};
        }
        case Type::kWildWild: {
            auto result =
                    knowledge_base->GetAllEntityIndices(StmtType::kAssign);
            return {assign_, {result.begin(), result.end()}};
        }
    }
}
}  // namespace spa
