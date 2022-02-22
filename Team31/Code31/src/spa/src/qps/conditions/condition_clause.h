#ifndef SRC_SPA_SRC_QPS_CONDITION_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITION_CLAUSE_H_

#include <utility>

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
class ConditionClause {
  public:
    virtual std::pair<ResultTable, bool> Execute(
            const KnowledgeBase* knowledge_base) const = 0;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITION_CLAUSE_H_
