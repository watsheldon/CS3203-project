#ifndef SPA_SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
#define SPA_SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_

#include "condition_clause.h"
#include "qps/query_token.h"
namespace spa {
class PatternClause : public ConditionClause {};
class PatternIdentExpr : public PatternClause {
  public:
    PatternIdentExpr(std::string first, std::vector<QueryToken> second);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class PatternIdentWild : public PatternClause {
  public:
    explicit PatternIdentWild(std::string first);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class PatternSynExpr : public PatternClause {
  public:
    PatternSynExpr(Synonym* first, std::vector<QueryToken> second);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class PatternSynWild : public PatternClause {
  public:
    explicit PatternSynWild(Synonym* first);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class PatternWildExpr : public PatternClause {
  public:
    explicit PatternWildExpr(std::vector<QueryToken> second);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class PatternWildWild : public PatternClause {
  public:
    PatternWildWild();
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
