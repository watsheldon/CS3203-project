#ifndef SRC_SPA_SRC_QPS_CONDITIONS_STMTSTMTBASE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_STMTSTMTBASE_H_

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "condition_clause.h"
#include "qps/synonym.h"

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
    static Type StmtStmtType(FirstParamType first, SecondParamType second);
    ~StmtStmtBase() override = default;

  protected:
    Type type_;
    int first_int_;
    int second_int_;
    Synonym *first_syn_;
    Synonym *second_syn_;
    static constexpr Array2D<Type, 3, 3> stmt_stmt_type_{
            {{Type::kIntInt, Type::kIntSyn, Type::kIntWild},
             {Type::kSynInt, Type::kSynSyn, Type::kSynWild},
             {Type::kWildInt, Type::kWildSyn, Type::kWildWild}}};
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_STMTSTMTBASE_H_
