#ifndef SPA_SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_CLAUSE_H_
#define SPA_SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_CLAUSE_H_

#include "condition_clause.h"
namespace spa {
class FollowsClause : public ConditionClause {};
class FollowsIntInt : public FollowsClause {};
class FollowsIntSyn : public FollowsClause {};
class FollowsIntWild : public FollowsClause {};
class FollowsSynInt : public FollowsClause {};
class FollowsSynSyn : public FollowsClause {};
class FollowsSynWild : public FollowsClause {};
class FollowsWildInt : public FollowsClause {};
class FollowsWildSyn : public FollowsClause {};
class FollowsWildWild : public FollowsClause {};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_QPS_CONDITIONS_FOLLOWS_CLAUSE_H_
