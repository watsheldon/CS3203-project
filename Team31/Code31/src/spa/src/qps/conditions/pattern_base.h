#ifndef SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_

#include <array>
#include <vector>

#include "common/aliases.h"
#include "condition_clause.h"
#include "qps/evaluator/result_table.h"
#include "qps/query_token.h"

namespace spa {
class PatternBase : public ConditionClause {
  public:
    using Expression = std::vector<QueryToken>;
    using VarName = IdentView;
    using FirstParam = std::variant<Wildcard, VarName, Synonym *>;
    using SecondParam = std::variant<Wildcard, Expression>;
    enum Type { kVarExpr, kVarWild, kSynExpr, kSynWild, kWildExpr, kWildWild };
    PatternBase(Synonym *assign, VarName first,
                std::vector<QueryToken> &&second) noexcept;  // IdentExpr
    PatternBase(Synonym *assign,
                VarName first) noexcept;  // IdentWild
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
        return kTypeMatrix[first_index][second_index];
    }
    ~PatternBase() override = default;
    ResultTable Execute(KnowledgeBase *pkb) const noexcept final;

  protected:
    Type type_;
    FirstParam first_param_;
    SecondParam second_param_;
    Synonym *zeroth_param_;
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
    ResultTable VarWild(KnowledgeBase *pkb, VarName first) const noexcept;
    ResultTable SynWild(KnowledgeBase *pkb, Synonym *first) const noexcept;
    ResultTable WildWild(KnowledgeBase *pkb) const noexcept;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
