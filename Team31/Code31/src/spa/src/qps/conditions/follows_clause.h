#ifndef SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_CLAUSE_H_

#include <string>

#include "condition_clause.h"
namespace spa {
class FollowsClause : public ConditionClause {};
class FollowsIntInt : public FollowsClause {
  public:
    FollowsIntInt(int first, int second, bool transitive);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    const int first_;
    const int second_;
    const bool transitive_;
};
class FollowsIntSyn : public FollowsClause {
  public:
    FollowsIntSyn(int first, Synonym *second, bool transitive);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    const int first_;
    Synonym *const second_;
    const bool transitive_;
};
class FollowsIntWild : public FollowsClause {
  public:
    explicit FollowsIntWild(int first);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    const int first_;
};
class FollowsSynInt : public FollowsClause {
  public:
    FollowsSynInt(Synonym *first, int second, bool transitive);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const first_;
    const int second_;
    const bool transitive_;
};
class FollowsSynSyn : public FollowsClause {
  public:
    FollowsSynSyn(Synonym *first, Synonym *second, bool transitive);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const first_;
    Synonym *const second_;
    const bool transitive_;
};
class FollowsSynWild : public FollowsClause {
  public:
    explicit FollowsSynWild(Synonym *first);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const first_;
};
class FollowsWildInt : public FollowsClause {
  public:
    explicit FollowsWildInt(int second);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    const int second_;
};
class FollowsWildSyn : public FollowsClause {
  public:
    explicit FollowsWildSyn(Synonym *second);
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const second_;
};
class FollowsWildWild : public FollowsClause {
  public:
    FollowsWildWild();
    std::pair<ResultTable, bool> Execute(
            KnowledgeBase *knowledge_base) const override;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_CLAUSE_H_
