#ifndef SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_

#include <string>

#include "condition_clause.h"
namespace spa {
class ModifiesClause : public ConditionClause {};
class ModifiesIntIdent : public ModifiesClause {
  public:
    ModifiesIntIdent(int first, std::string second);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    const int first_;
    const std::string second_;
};
class ModifiesIntSyn : public ModifiesClause {
  public:
    ModifiesIntSyn(int first, Synonym *second);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    const int first_;
    Synonym *const second_;
};
class ModifiesIntWild : public ModifiesClause {
  public:
    explicit ModifiesIntWild(int first);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    const int first_;
};
class ModifiesSynIdent : public ModifiesClause {
  public:
    ModifiesSynIdent(Synonym *first, std::string second);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const first_;
    const std::string second_;
};
class ModifiesSynSyn : public ModifiesClause {
  public:
    ModifiesSynSyn(Synonym *first, Synonym *second);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const first_;
    Synonym *const second_;
};
class ModifiesSynWild : public ModifiesClause {
  public:
    explicit ModifiesSynWild(Synonym *first);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;

  private:
    Synonym *const first_;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_
