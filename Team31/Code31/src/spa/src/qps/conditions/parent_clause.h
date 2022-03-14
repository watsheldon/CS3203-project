#ifndef SRC_SPA_SRC_QPS_PARENT_CLAUSE_H_
#define SRC_SPA_SRC_QPS_PARENT_CLAUSE_H_

#include "qps/conditions/condition_clause.h"

namespace spa {
class ParentClause : public ConditionClause {
  public:
    ParentClause();
    ParentClause(int first, int second);
    ParentClause(int first, Synonym *second);
    ParentClause(Synonym *first, int second);
    ParentClause(Synonym *first, Synonym *second);
    ParentClause(ArgPos pos, int integer);
    ParentClause(ArgPos pos, Synonym *syn);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    Type type_;
    int first_int_;
    int second_int_;
    Synonym *first_syn_;
    Synonym *second_syn_;
};

class ParentTransClause : public ConditionClause {
  public:
    ParentTransClause();
    ParentTransClause(int first, int second);
    ParentTransClause(int first, Synonym *second);
    ParentTransClause(Synonym *first, int second);
    ParentTransClause(Synonym *first, Synonym *second);
    ParentTransClause(ArgPos pos, int integer);
    ParentTransClause(ArgPos pos, Synonym *syn);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    Type type_;
    int first_int_;
    int second_int_;
    Synonym *first_syn_;
    Synonym *second_syn_;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_PARENT_CLAUSE_H_
