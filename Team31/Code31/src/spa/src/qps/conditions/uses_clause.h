#ifndef SPA_SRC_SPA_SRC_QPS_USES_MODIFIES_CLAUSE_H_
#define SPA_SRC_SPA_SRC_QPS_USES_MODIFIES_CLAUSE_H_

#include "qps/conditions/condition_clause.h"

namespace spa {
class UsesClause : public ConditionClause {};
class UsesIntIdent : public UsesClause {};
class UsesIntSyn : public UsesClause {};
class UsesIntWild : public UsesClause {};
class UsesSynIdent : public UsesClause {};
class UsesSynSyn : public UsesClause {};
class UsesSynWild : public UsesClause {};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_QPS_USES_MODIFIES_CLAUSE_H_
