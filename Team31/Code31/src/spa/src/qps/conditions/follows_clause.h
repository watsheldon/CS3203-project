#ifndef SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_CLAUSE_H_

#include <string>

#include "condition_clause.h"
namespace spa {

class FollowsClause : public ConditionClause {
  public:
    FollowsClause();
    FollowsClause(int first, int second);
    FollowsClause(int first, Synonym *second);
    FollowsClause(Synonym *first, int second);
    FollowsClause(Synonym *first, Synonym *second);
    FollowsClause(ArgPos pos, int integer);
    FollowsClause(ArgPos pos, Synonym *syn);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    Type type_;
    int first_int_;
    int second_int_;
    Synonym *first_syn_;
    Synonym *second_syn_;
};

class FollowsTransClause : public ConditionClause {
  public:
    FollowsTransClause();
    FollowsTransClause(int first, int second);
    FollowsTransClause(int first, Synonym *second);
    FollowsTransClause(Synonym *first, int second);
    FollowsTransClause(Synonym *first, Synonym *second);
    FollowsTransClause(ArgPos pos, int integer);
    FollowsTransClause(ArgPos pos, Synonym *syn);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    Type type_;
    int first_int_;
    int second_int_;
    Synonym *first_syn_;
    Synonym *second_syn_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_CLAUSE_H_
