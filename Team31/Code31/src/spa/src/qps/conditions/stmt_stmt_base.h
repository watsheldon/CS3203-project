#ifndef SRC_SPA_SRC_QPS_CONDITIONS_STMTSTMTBASE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_STMTSTMTBASE_H_

#include "condition_clause.h"

namespace spa {
class StmtStmtBase : public ConditionClause {
  public:
    enum Type {
        kIntInt,
        kIntSyn,
        kIntWild,
        kSynInt,
        kSynSyn,
        kSynWild,
        kWildInt,
        kWildSyn,
        kWildWild,
    };

    StmtStmtBase();
    StmtStmtBase(int first, int second);
    StmtStmtBase(int first, Synonym *second);
    StmtStmtBase(Synonym *first, int second);
    StmtStmtBase(Synonym *first, Synonym *second);
    StmtStmtBase(ArgPos pos, int integer);
    StmtStmtBase(ArgPos pos, Synonym *syn);
    ResultTable Execute(KnowledgeBase *knowledge_base) const override = 0;
    ~StmtStmtBase() override = 0;

  protected:
    Type type_;
    int first_int_;
    int second_int_;
    Synonym *first_syn_;
    Synonym *second_syn_;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_STMTSTMTBASE_H_
