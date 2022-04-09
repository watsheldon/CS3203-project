#ifndef SRC_SPA_SRC_QPS_CONDITION_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITION_CLAUSE_H_

#include <variant>

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
class ConditionClause {
  public:
    using Wildcard = std::monostate;
    using ProcName = IdentView;
    using VarName = IdentView;
    virtual ResultTable Execute(KnowledgeBase *pkb) const noexcept = 0;
    virtual ~ConditionClause() = 0;
    bool operator<(const ConditionClause &other) const noexcept;
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

  protected:
    [[nodiscard]] virtual int GetSynCount() const noexcept = 0;
    [[nodiscard]] virtual int GetPriority() const noexcept = 0;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITION_CLAUSE_H_
