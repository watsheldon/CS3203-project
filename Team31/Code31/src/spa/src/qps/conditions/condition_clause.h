#ifndef SRC_SPA_SRC_QPS_CONDITION_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITION_CLAUSE_H_

#include <utility>

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
class ConditionClause {
  public:
    virtual ResultTable Execute(KnowledgeBase *knowledge_base) const = 0;
    virtual ~ConditionClause() = 0;
    enum class FirstParamType {
        kInt,
        kSyn,
        kWild,
        kIdent,
    };
    enum class SecondParamType {
        kInt,
        kSyn,
        kWild,
        kExpr,
        kIdent,
    };
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITION_CLAUSE_H_
