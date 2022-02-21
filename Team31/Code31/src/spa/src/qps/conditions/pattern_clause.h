#ifndef SPA_SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
#define SPA_SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_

#include "condition_clause.h"
namespace spa {
class PatternClause : public ConditionClause {};
class PatternIdentExpr : public PatternClause {};
class PatternIdentWild : public PatternClause {};
class PatternSynExpr : public PatternClause {};
class PatternSynWild : public PatternClause {};
class PatternWildExpr : public PatternClause {};
class PatternWildWild : public PatternClause {};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_CONDITIONS_PATTERN_CLAUSE_H_
