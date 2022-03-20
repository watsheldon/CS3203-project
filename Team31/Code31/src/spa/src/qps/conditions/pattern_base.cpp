#include "pattern_base.h"

#include <string>
#include <utility>
#include <vector>

#include "qps/query_token.h"
#include "qps/synonym.h"

namespace spa {
PatternBase::PatternBase(Synonym *assign, std::string first,
                         std::vector<QueryToken> &&second) noexcept
        : type_(Type::kIdentExpr),
          assign_(assign),
          first_ident_(std::move(first)),
          second_expr_(std::move(second)) {}
PatternBase::PatternBase(Synonym *const assign, std::string first) noexcept
        : type_(Type::kIdentWild),
          assign_(assign),
          first_ident_(std::move(first)) {}
PatternBase::PatternBase(Synonym *const assign, Synonym *first,
                         std::vector<QueryToken> &&second) noexcept
        : type_(Type::kSynExpr),
          assign_(assign),
          first_syn_(first),
          second_expr_(std::move(second)) {}
PatternBase::PatternBase(Synonym *const assign, Synonym *first) noexcept
        : type_(Type::kSynWild), assign_(assign), first_syn_(first) {}
PatternBase::PatternBase(Synonym *const assign,
                         std::vector<QueryToken> &&second) noexcept
        : type_(Type::kWildExpr),
          assign_(assign),
          second_expr_(std::move(second)) {}
PatternBase::PatternBase(Synonym *const assign) noexcept
        : type_(Type::kWildWild), assign_(assign) {}
}  // namespace spa
