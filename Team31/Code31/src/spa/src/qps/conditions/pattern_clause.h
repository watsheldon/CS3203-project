#ifndef SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_

#include "condition_clause.h"
#include "qps/query_token.h"
namespace spa {
class PatternClause : public ConditionClause {};
class PatternIdentExpr : public PatternClause {
  public:
    PatternIdentExpr(std::string first, std::vector<QueryToken> &&second,
                     bool partial);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    const std::string first_;
    const std::vector<QueryToken> second_;
    const bool partial_;
};
class PatternIdentWild : public PatternClause {
  public:
    explicit PatternIdentWild(std::string first);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    const std::string first_;
};
class PatternSynExpr : public PatternClause {
  public:
    PatternSynExpr(Synonym *first, std::vector<QueryToken> &&second,
                   bool partial);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const first_;
    const std::vector<QueryToken> second_;
    const bool partial_;
};
class PatternSynWild : public PatternClause {
  public:
    explicit PatternSynWild(Synonym *first);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const first_;
};
class PatternWildExpr : public PatternClause {
  public:
    explicit PatternWildExpr(std::vector<QueryToken> &&second, bool partial);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    const std::vector<QueryToken> second_;
    const bool partial_;
};
class PatternWildWild : public PatternClause {
  public:
    PatternWildWild();
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
