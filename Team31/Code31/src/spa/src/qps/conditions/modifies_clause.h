#ifndef SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_

#include <string>

#include "condition_clause.h"
namespace spa {
class ModifiesClause : public ConditionClause {
  public:
    ModifiesClause(int first, std::string second);
    ModifiesClause(int first, Synonym *second);
    explicit ModifiesClause(int first);
    ModifiesClause(Synonym *first, std::string second);
    ModifiesClause(Synonym *first, Synonym *second);
    explicit ModifiesClause(Synonym *first);
    ModifiesClause(std::string first, Synonym *second);
    ModifiesClause(std::string first, std::string second);
    explicit ModifiesClause(std::string first);
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
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_
