#ifndef SRC_SPA_SRC_QPS_USES_MODIFIES_CLAUSE_H_
#define SRC_SPA_SRC_QPS_USES_MODIFIES_CLAUSE_H_

#include "qps/conditions/condition_clause.h"

namespace spa {
class UsesClause : public ConditionClause {};
class UsesIntIdent : public UsesClause {
  public:
    UsesIntIdent(int first, std::string second);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    const int first_;
    const std::string second_;
};
class UsesIntSyn : public UsesClause {
  public:
    UsesIntSyn(int first, Synonym *second);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    const int first_;
    Synonym *const second_;
};
class UsesIntWild : public UsesClause {
  public:
    explicit UsesIntWild(int first);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    const int first_;
};
class UsesSynIdent : public UsesClause {
  public:
    UsesSynIdent(Synonym *first, std::string second);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const first_;
    const std::string second_;
};
class UsesSynSyn : public UsesClause {
  public:
    UsesSynSyn(Synonym *first, Synonym *second);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const first_;
    Synonym *const second_;
};
class UsesSynWild : public UsesClause {
  public:
    explicit UsesSynWild(Synonym *first);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const first_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_USES_MODIFIES_CLAUSE_H_
