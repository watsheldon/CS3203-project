#ifndef SPA_SRC_SPA_SRC_QPS_USES_MODIFIES_CLAUSE_H_
#define SPA_SRC_SPA_SRC_QPS_USES_MODIFIES_CLAUSE_H_

#include "qps/conditions/condition_clause.h"

namespace spa {
class UsesClause : public ConditionClause {};
class UsesIntIdent : public UsesClause {
  public:
    UsesIntIdent(int first, std::string second);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class UsesIntSyn : public UsesClause {
  public:
    UsesIntSyn(int first, Synonym* second);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class UsesIntWild : public UsesClause {
  public:
    explicit UsesIntWild(int first);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class UsesSynIdent : public UsesClause {
  public:
    UsesSynIdent(Synonym* first, std::string second);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class UsesSynSyn : public UsesClause {
  public:
    UsesSynSyn(Synonym* first, Synonym* second);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class UsesSynWild : public UsesClause {
  public:
    UsesSynWild(Synonym* first);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_QPS_USES_MODIFIES_CLAUSE_H_
