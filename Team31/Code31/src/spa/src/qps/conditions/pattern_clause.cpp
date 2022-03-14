#include "pattern_clause.h"

#include <string>
#include <utility>
#include <vector>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/type_convert_helpers.h"
#include "qps/query_token.h"

namespace spa {
PatternPartialClause::PatternPartialClause(Synonym *assign, std::string first,
                                           std::vector<QueryToken> &&second)
        : type_(Type::kIdentExpr),
          assign_(assign),
          first_ident_(std::move(first)),
          second_expr_(std::move(second)) {}
PatternPartialClause::PatternPartialClause(Synonym *const assign,
                                           std::string first)
        : type_(Type::kIdentWild),
          assign_(assign),
          first_ident_(std::move(first)) {}
PatternPartialClause::PatternPartialClause(Synonym *const assign,
                                           Synonym *first,
                                           std::vector<QueryToken> &&second)
        : type_(Type::kSynExpr),
          assign_(assign),
          first_syn_(first),
          second_expr_(std::move(second)) {}
PatternPartialClause::PatternPartialClause(Synonym *const assign,
                                           Synonym *first)
        : type_(Type::kSynWild), assign_(assign), first_syn_(first) {}
PatternPartialClause::PatternPartialClause(Synonym *const assign,
                                           std::vector<QueryToken> &&second)
        : type_(Type::kWildExpr),
          assign_(assign),
          second_expr_(std::move(second)) {}
PatternPartialClause::PatternPartialClause() : type_(Type::kWildWild) {}
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
PatternExactClause::PatternExactClause(Synonym *assign, std::string first,
                                       std::vector<QueryToken> &&second)
        : type_(Type::kIdentExpr),
          assign_(assign),
          first_ident_(std::move(first)),
          second_expr_(std::move(second)) {}
PatternExactClause::PatternExactClause(Synonym *assign, Synonym *first,
                                       std::vector<QueryToken> &&second)
        : type_(Type::kSynExpr),
          assign_(assign),
          first_syn_(first),
          second_expr_(std::move(second)) {}
PatternExactClause::PatternExactClause(Synonym *assign,
                                       std::vector<QueryToken> &&second)
        : type_(Type::kWildExpr),
          assign_(assign),
          second_expr_(std::move(second)) {}

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
    }
}
}  // namespace spa
