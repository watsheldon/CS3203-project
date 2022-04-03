#ifndef SPA_SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_BASE_H_
#define SPA_SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_BASE_H_

#include "condition_clause.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
class PatternBase : public ConditionClause {
  public:
    using Expression = std::vector<QueryToken>;
    using FirstParam = std::variant<Wildcard, VarName, Synonym *>;
    enum Type { kVarExpr, kVarWild, kSynExpr, kSynWild, kWildExpr, kWildWild };
    PatternBase(Synonym *syn,
                VarName first) noexcept;                 // IdentWild
    PatternBase(Synonym *syn, Synonym *first) noexcept;  // SynWild
    explicit PatternBase(Synonym *syn) noexcept;         // WildWild
    static constexpr Type GetType(FirstParamType first,
                                  SecondParamType second) noexcept {
        int first_index = static_cast<int>(first) - 1;
        int second_index = static_cast<int>(second) - 2;
        return kTypeMatrix[first_index][second_index];
    }
    ~PatternBase() override = default;
    ResultTable Execute(KnowledgeBase *pkb) const noexcept override;

  protected:
    Type type_;
    FirstParam first_param_;
    Synonym *zeroth_param_;
    static constexpr Array2D<Type, 3, 2> kTypeMatrix = {
            {{Type::kSynWild, Type::kSynExpr},
             {Type::kWildWild, Type::kWildExpr},
             {Type::kVarWild, Type::kVarExpr}}};
    PatternBase(Type type, Synonym *zeroth,
                FirstParam first_param = Wildcard()) noexcept;
    virtual ResultTable VarWild(KnowledgeBase *pkb,
                                VarName first) const noexcept;
    virtual ResultTable SynWild(KnowledgeBase *pkb,
                                Synonym *first) const noexcept;
    virtual ResultTable WildWild(KnowledgeBase *pkb) const noexcept;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_BASE_H_
