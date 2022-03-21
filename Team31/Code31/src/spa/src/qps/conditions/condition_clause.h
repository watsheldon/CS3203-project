#ifndef SRC_SPA_SRC_QPS_CONDITION_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITION_CLAUSE_H_

#include <variant>

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
class ConditionClause {
  public:
    using Wildcard = std::monostate;
    virtual ResultTable Execute(
            KnowledgeBase *knowledge_base) const noexcept = 0;
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
