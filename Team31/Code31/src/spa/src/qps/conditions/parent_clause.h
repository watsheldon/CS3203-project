#ifndef SPA_SRC_SPA_SRC_QPS_PARENT_FOLLOWS_CLAUSE_H_
#define SPA_SRC_SPA_SRC_QPS_PARENT_FOLLOWS_CLAUSE_H_

#include "qps/conditions/condition_clause.h"

namespace spa {
class ParentClause : public ConditionClause {};
class ParentIntInt : public ParentClause {};
class ParentIntSyn : public ParentClause {};
class ParentIntWild : public ParentClause {};
class ParentSynInt : public ParentClause {};
class ParentSynSyn : public ParentClause {};
class ParentSynWild : public ParentClause {};
class ParentWildInt : public ParentClause {};
class ParentWildSyn : public ParentClause {};
class ParentWildWild : public ParentClause {};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_PARENT_FOLLOWS_CLAUSE_H_
