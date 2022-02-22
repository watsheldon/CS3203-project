#ifndef SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_CLAUSE_H_

#include <string>
#include "condition_clause.h"
namespace spa {
class FollowsClause : public ConditionClause {};
class FollowsIntInt : public FollowsClause {
  public:
    FollowsIntInt(int first, int second, bool isTrans);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class FollowsIntSyn : public FollowsClause {
  public:
    FollowsIntSyn(int first, Synonym* second, bool isTrans);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class FollowsIntWild : public FollowsClause {
  public:
    FollowsIntWild(int first, bool isTrans);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class FollowsSynInt : public FollowsClause {
  public:
    FollowsSynInt(Synonym* first, int second, bool isTrans);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class FollowsSynSyn : public FollowsClause {
  public:
    FollowsSynSyn(Synonym* first, Synonym* second, bool isTrans);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class FollowsSynWild : public FollowsClause {
  public:
    FollowsSynWild(Synonym* first, bool isTrans);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class FollowsWildInt : public FollowsClause {
  public:
    FollowsWildInt(int second, bool isTrans);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class FollowsWildSyn : public FollowsClause {
  public:
    FollowsWildSyn(Synonym* second, bool isTrans);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class FollowsWildWild : public FollowsClause {
  public:
    FollowsWildWild();
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_CLAUSE_H_
