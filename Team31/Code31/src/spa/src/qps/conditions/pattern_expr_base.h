#ifndef SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_

#include <array>
#include <vector>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "pattern_base.h"
#include "qps/evaluator/result_table.h"
#include "qps/query_token.h"

namespace spa {
class PatternExprBase : public PatternBase {
  public:
    using SecondParam = std::variant<Wildcard, Expression>;
    PatternExprBase(Synonym *syn, VarName first,
                    std::vector<QueryToken> &&second) noexcept;  // IdentExpr
    PatternExprBase(Synonym *syn, Synonym *first,
                    std::vector<QueryToken> &&second) noexcept;  // SynExpr
    PatternExprBase(Synonym *syn,
                    std::vector<QueryToken> &&second) noexcept;  // WildExpr
    static constexpr Type GetType(FirstParamType first,
                                  SecondParamType second) noexcept {
        int first_index = static_cast<int>(first) - 1;
        int second_index = static_cast<int>(second) - 2;
        return kTypeMatrix[first_index][second_index];
    }
    ResultTable Execute(KnowledgeBase *pkb) const noexcept final;

  protected:
    using PatternBase::PatternBase;
    SecondParam second_param_;
    static constexpr Array2D<Type, 3, 2> kTypeMatrix = {
            {{Type::kSynWild, Type::kSynExpr},
             {Type::kWildWild, Type::kWildExpr},
             {Type::kVarWild, Type::kVarExpr}}};
    virtual ResultTable VarExpr(KnowledgeBase *pkb, VarName first,
                                Expression second) const noexcept = 0;
    virtual ResultTable SynExpr(KnowledgeBase *pkb, Synonym *first,
                                Expression second) const noexcept = 0;
    virtual ResultTable WildExpr(KnowledgeBase *pkb,
                                 Expression second) const noexcept = 0;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
