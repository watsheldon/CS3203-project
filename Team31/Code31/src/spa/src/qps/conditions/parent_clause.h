#ifndef SRC_SPA_SRC_QPS_PARENT_FOLLOWS_CLAUSE_H_
#define SRC_SPA_SRC_QPS_PARENT_FOLLOWS_CLAUSE_H_

#include "qps/conditions/condition_clause.h"

namespace spa {
class ParentClause : public ConditionClause {};
class ParentIntInt : public ParentClause {
  public:
    ParentIntInt(int first, int second, bool isTrans);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class ParentIntSyn : public ParentClause {
  public:
    ParentIntSyn(int first, Synonym* second, bool isTrans);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class ParentIntWild : public ParentClause {
  public:
    ParentIntWild(int first, bool isTrans);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class ParentSynInt : public ParentClause {
  public:
    ParentSynInt(Synonym* first, int second, bool isTrans);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class ParentSynSyn : public ParentClause {
  public:
    ParentSynSyn(Synonym* first, Synonym* second, bool isTrans);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class ParentSynWild : public ParentClause {
  public:
    ParentSynWild(Synonym* first, bool isTrans);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class ParentWildInt : public ParentClause {
  public:
    ParentWildInt(int second, bool isTrans);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class ParentWildSyn : public ParentClause {
  public:
    ParentWildSyn(Synonym* second, bool isTrans);
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
class ParentWildWild : public ParentClause {
  public:
    ParentWildWild();
    std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const override;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_PARENT_FOLLOWS_CLAUSE_H_
