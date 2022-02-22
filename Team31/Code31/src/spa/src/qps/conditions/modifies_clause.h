#ifndef SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_

#include <string>

#include "condition_clause.h"
namespace spa {
class ModifiesClause : public ConditionClause {};
class ModifiesIntIdent : public ModifiesClause {
  public:
    ModifiesIntIdent(int first, std::string second);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class ModifiesIntSyn : public ModifiesClause {
  public:
    ModifiesIntSyn(int first, Synonym* second);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class ModifiesIntWild : public ModifiesClause {
  public:
    explicit ModifiesIntWild(int first);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class ModifiesSynIdent : public ModifiesClause {
  public:
    ModifiesSynIdent(Synonym* first, std::string second);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class ModifiesSynSyn : public ModifiesClause {
  public:
    ModifiesSynSyn(Synonym* first, Synonym* second);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class ModifiesSynWild : public ModifiesClause {
  public:
    explicit ModifiesSynWild(Synonym* first);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_
