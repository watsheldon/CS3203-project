#ifndef SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
#define SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_

#include <memory>
#include <vector>

#include "synonym.h"
#include "qps/conditions/condition_clause.h"

namespace spa {
class QueryObject {
  public:
    const bool isValidQuery;
    const std::vector<Synonym* const> synonyms;
    const Synonym* select;
    std::vector<std::unique_ptr<ConditionClause>> conditions;
    QueryObject(bool isValid, std::vector<Synonym* const>& syns,
                const Synonym* sel,
                std::vector<std::unique_ptr<ConditionClause>>& cons)
            : isValidQuery(isValid), synonyms(std::move(syns)), select(sel), conditions(std::move(cons)) {}
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
