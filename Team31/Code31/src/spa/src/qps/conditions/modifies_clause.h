#ifndef SPA_SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_
#define SPA_SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_

#include "condition_clause.h"
namespace spa {
class ModifiesClause : public ConditionClause {};
class ModifiesIntIdent : public ModifiesClause {};
class ModifiesIntSyn : public ModifiesClause {};
class ModifiesIntWild : public ModifiesClause {};
class ModifiesSynIdent : public ModifiesClause {};
class ModifiesSynSyn : public ModifiesClause {};
class ModifiesSynWild : public ModifiesClause {};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_
