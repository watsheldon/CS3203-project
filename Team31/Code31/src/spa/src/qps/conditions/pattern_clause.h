#ifndef SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_

#include "condition_clause.h"
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
    PatternBase(Synonym *const assign,
                std::string first);  // IdentWild
    PatternBase(Synonym *const assign, Synonym *first,
                std::vector<QueryToken> &&second);       // SynExpr
    PatternBase(Synonym *const assign, Synonym *first);  // SynWild
    PatternBase(Synonym *const assign,
                std::vector<QueryToken> &&second);  // WildExpr
    PatternBase();                                  // WildWild
    ResultTable Execute(KnowledgeBase *knowledge_base) const override = 0;
    virtual ~PatternBase() = 0;

  protected:
    Type type_;
    Synonym *assign_;
    std::string first_ident_;
    Synonym *first_syn_;
    std::vector<QueryToken> second_expr_;
};
class PatternPartial : public PatternBase {
    using PatternBase::PatternBase;

  public:
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;
};

class PatternExact : public PatternBase {
    using PatternBase::PatternBase;

  public:
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
