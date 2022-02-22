#ifndef SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_

#include "condition_clause.h"
#include "qps/query_token.h"
namespace spa {
class PatternClause : public ConditionClause {};
class PatternIdentExpr : public PatternClause {
  public:
    PatternIdentExpr(Synonym *const assign, std::string first,
                     std::vector<QueryToken> &&second, bool partial);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const assign_;
    const std::string first_;
    const std::vector<QueryToken> second_;
    const bool partial_;
};
class PatternIdentWild : public PatternClause {
  public:
    explicit PatternIdentWild(Synonym *const assign, std::string first);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const assign_;
    const std::string first_;
};
class PatternSynExpr : public PatternClause {
  public:
    PatternSynExpr(Synonym *const assign, Synonym *first,
                   std::vector<QueryToken> &&second, bool partial);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const assign_;
    Synonym *const first_;
    const std::vector<QueryToken> second_;
    const bool partial_;
};
class PatternSynWild : public PatternClause {
  public:
    explicit PatternSynWild(Synonym *const assign, Synonym *first);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const assign_;
    Synonym *const first_;
};
class PatternWildExpr : public PatternClause {
  public:
    explicit PatternWildExpr(Synonym *const assign,
                             std::vector<QueryToken> &&second, bool partial);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const assign_;
    const std::vector<QueryToken> second_;
    const bool partial_;
};
class PatternWildWild : public PatternClause {
  public:
    PatternWildWild(Synonym *const assign);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const assign_;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
