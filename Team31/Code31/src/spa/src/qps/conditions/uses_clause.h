#ifndef SRC_SPA_SRC_QPS_USES_MODIFIES_CLAUSE_H_
#define SRC_SPA_SRC_QPS_USES_MODIFIES_CLAUSE_H_

#include "qps/conditions/condition_clause.h"

namespace spa {
class UsesClause : public ConditionClause {
  public:
    UsesClause(int first, std::string second);
    UsesClause(int first, Synonym *second);
    explicit UsesClause(int first);
    UsesClause(Synonym *first, std::string second);
    UsesClause(Synonym *first, Synonym *second);
    explicit UsesClause(Synonym *first);
    UsesClause(std::string first, Synonym *second);
    UsesClause(std::string first, std::string second);
    explicit UsesClause(std::string first);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    Type type_;
    int first_int_;
    Synonym *first_syn_;
    std::string first_ident_;
    int second_int_;
    Synonym *second_syn_;
    std::string second_ident_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_USES_MODIFIES_CLAUSE_H_
