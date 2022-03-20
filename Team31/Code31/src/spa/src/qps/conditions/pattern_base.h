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
                std::vector<QueryToken> &&second);  // IdentExpr
    PatternBase(Synonym *assign,
                std::string first);  // IdentWild
    PatternBase(Synonym *assign, Synonym *first,
                std::vector<QueryToken> &&second);  // SynExpr
    PatternBase(Synonym *assign, Synonym *first);   // SynWild
    PatternBase(Synonym *assign,
                std::vector<QueryToken> &&second);  // WildExpr
    explicit PatternBase(Synonym *assign);          // WildWild
    ResultTable Execute(KnowledgeBase *knowledge_base) const override = 0;
    static Type PatternType(FirstParamType first, SecondParamType second);
    ~PatternBase() override = 0;

  protected:
    Type type_;
    Synonym *assign_;
    std::string first_ident_;
    Synonym *first_syn_;
    std::vector<QueryToken> second_expr_;
    static constexpr Array2D<Type, 3, 2> pattern_type_ = {
            {{Type::kSynWild, Type::kSynExpr},
             {Type::kWildWild, Type::kWildExpr},
             {Type::kIdentWild, Type::kIdentExpr}}};
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
