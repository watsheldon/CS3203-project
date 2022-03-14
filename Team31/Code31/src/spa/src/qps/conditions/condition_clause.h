#ifndef SRC_SPA_SRC_QPS_CONDITION_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITION_CLAUSE_H_

#include <utility>

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
enum class Type {
    kIntInt,
    kIntSyn,
    kIntWild,
    kSynInt,
    kSynSyn,
    kSynWild,
    kWildInt,
    kWildSyn,
    kWildWild
};
class ConditionClause {
  public:
    virtual ResultTable Execute(KnowledgeBase *knowledge_base) const = 0;
    virtual ~ConditionClause() = 0;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITION_CLAUSE_H_
