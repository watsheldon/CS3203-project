#ifndef SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_

#include "condition_clause.h"
#include "qps/query_token.h"
namespace spa {
class PatternPartialClause : public ConditionClause {
  public:
    PatternPartialClause(Synonym *assign, std::string first,
                         std::vector<QueryToken> &&second);  // IdentExpr
    PatternPartialClause(Synonym *const assign,
                         std::string first);  // IdentWild
    PatternPartialClause(Synonym *const assign, Synonym *first,
                         std::vector<QueryToken> &&second);       // SynExpr
    PatternPartialClause(Synonym *const assign, Synonym *first);  // SynWild
    PatternPartialClause(Synonym *const assign,
                         std::vector<QueryToken> &&second);  // WildExpr
    PatternPartialClause();                                  // WildWild
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    Type type_;
    Synonym *assign_;
    std::string first_ident_;
    Synonym *first_syn_;
    std::vector<QueryToken> second_expr_;
};

class PatternExactClause : public ConditionClause {
  public:
    PatternExactClause(Synonym *assign, std::string first,
                       std::vector<QueryToken> &&second);  // identExpr
    PatternExactClause(Synonym *const assign, Synonym *first,
                       std::vector<QueryToken> &&second);  // SynExpr
    PatternExactClause(Synonym *const assign,
                       std::vector<QueryToken> &&second);  // WildExpr
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    Type type_;
    Synonym *assign_;
    std::string first_ident_;
    Synonym *first_syn_;
    std::vector<QueryToken> second_expr_;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
