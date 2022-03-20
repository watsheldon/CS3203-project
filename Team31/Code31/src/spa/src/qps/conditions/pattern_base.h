#ifndef SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_

#include <vector>

#include "common/aliases.h"
#include "condition_clause.h"
#include "qps/evaluator/result_table.h"
#include "qps/query_token.h"

namespace spa {
class PatternBase : public ConditionClause {
  public:
    enum Type {
        kIdentExpr,
        kIdentWild,
        kSynExpr,
        kSynWild,
        kWildExpr,
        kWildWild
    };
    PatternBase(Synonym *assign, std::string first,
                std::vector<QueryToken> &&second) noexcept;  // IdentExpr
    PatternBase(Synonym *assign,
                std::string first) noexcept;  // IdentWild
    PatternBase(Synonym *assign, Synonym *first,
                std::vector<QueryToken> &&second) noexcept;  // SynExpr
    PatternBase(Synonym *assign, Synonym *first) noexcept;   // SynWild
    PatternBase(Synonym *assign,
                std::vector<QueryToken> &&second) noexcept;  // WildExpr
    explicit PatternBase(Synonym *assign) noexcept;          // WildWild
    static constexpr Type GetType(FirstParamType first,
                                  SecondParamType second) noexcept {
        int first_index = static_cast<int>(first) - 1;
        int second_index = static_cast<int>(second) - 2;
        return pattern_type_[first_index][second_index];
    }
    ~PatternBase() override = default;

  protected:
    Type type_;
    Synonym *assign_ = nullptr;
    std::string first_ident_;
    Synonym *first_syn_ = nullptr;
    std::vector<QueryToken> second_expr_;
    static constexpr Array2D<Type, 3, 2> pattern_type_ = {
            {{Type::kSynWild, Type::kSynExpr},
             {Type::kWildWild, Type::kWildExpr},
             {Type::kIdentWild, Type::kIdentExpr}}};
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
