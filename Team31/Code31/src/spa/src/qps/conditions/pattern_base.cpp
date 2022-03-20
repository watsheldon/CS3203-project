#include "pattern_base.h"

#include <string>
#include <utility>
#include <vector>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/type_convert_helpers.h"
#include "qps/query_token.h"

namespace spa {
PatternBase::PatternBase(Synonym *assign, std::string first,
                         std::vector<QueryToken> &&second)
        : type_(Type::kIdentExpr),
          assign_(assign),
          first_ident_(std::move(first)),
          second_expr_(std::move(second)) {}
PatternBase::PatternBase(Synonym *const assign, std::string first)
        : type_(Type::kIdentWild),
          assign_(assign),
          first_ident_(std::move(first)) {}
PatternBase::PatternBase(Synonym *const assign, Synonym *first,
                         std::vector<QueryToken> &&second)
        : type_(Type::kSynExpr),
          assign_(assign),
          first_syn_(first),
          second_expr_(std::move(second)) {}
PatternBase::PatternBase(Synonym *const assign, Synonym *first)
        : type_(Type::kSynWild), assign_(assign), first_syn_(first) {}
PatternBase::PatternBase(Synonym *const assign,
                         std::vector<QueryToken> &&second)
        : type_(Type::kWildExpr),
          assign_(assign),
          second_expr_(std::move(second)) {}
PatternBase::PatternBase(Synonym *const assign)
        : type_(Type::kWildWild), assign_(assign) {}
PatternBase::Type PatternBase::PatternType(
        ConditionClause::FirstParamType first,
        ConditionClause::SecondParamType second) {
    int first_index = static_cast<int>(first) - 1;
    int second_index = static_cast<int>(second) - 2;
    return pattern_type_[first_index][second_index];
}
PatternBase::~PatternBase() = default;
}  // namespace spa
