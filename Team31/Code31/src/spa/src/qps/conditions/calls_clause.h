#ifndef SRC_SPA_SRC_QPS_CONDITIONS_CALLS_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_CALLS_CLAUSE_H_

#include "calls_base.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
class CallsClause : public CallsBase {
    using CallsBase::CallsBase;
    ResultTable Execute(KnowledgeBase *knowledge_base) const noexcept final;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_CONDITIONS_CALLS_CLAUSE_H_
