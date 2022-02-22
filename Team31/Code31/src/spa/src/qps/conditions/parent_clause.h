#ifndef SRC_SPA_SRC_QPS_PARENT_FOLLOWS_CLAUSE_H_
#define SRC_SPA_SRC_QPS_PARENT_FOLLOWS_CLAUSE_H_

#include "qps/conditions/condition_clause.h"

namespace spa {
class ParentClause : public ConditionClause {};
class ParentIntInt : public ParentClause {
  public:
    ParentIntInt(int first, int second, bool transitive);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    const int first_;
    const int second_;
    const bool transitive_;
};
class ParentIntSyn : public ParentClause {
  public:
    ParentIntSyn(int first, Synonym *second, bool transitive);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    const int first_;
    Synonym *const second_;
    const bool transitive_;
};
class ParentIntWild : public ParentClause {
  public:
    explicit ParentIntWild(int first);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    const int first_;
};
class ParentSynInt : public ParentClause {
  public:
    ParentSynInt(Synonym *first, int second, bool transitive);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const first_;
    const int second_;
    const bool transitive_;
};
class ParentSynSyn : public ParentClause {
  public:
    ParentSynSyn(Synonym *first, Synonym *second, bool transitive);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const first_;
    Synonym *const second_;
    const bool transitive_;
};
class ParentSynWild : public ParentClause {
  public:
    explicit ParentSynWild(Synonym *first);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const first_;
};
class ParentWildInt : public ParentClause {
  public:
    explicit ParentWildInt(int second);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    const int second_;
};
class ParentWildSyn : public ParentClause {
  public:
    explicit ParentWildSyn(Synonym *second);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const second_;
};
class ParentWildWild : public ParentClause {
  public:
    ParentWildWild();
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_PARENT_FOLLOWS_CLAUSE_H_
