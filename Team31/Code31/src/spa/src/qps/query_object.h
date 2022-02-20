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
    const std::vector<std::unique_ptr<Synonym>> synonyms;
    const std::unique_ptr<Synonym> select;
    std::vector<std::unique_ptr<ConditionClause>> conditions;
    QueryObject(bool isValid, std::vector<std::unique_ptr<Synonym>> syns,
                std::unique_ptr<Synonym> sel,
                std::vector<std::unique_ptr<ConditionClause>> cons)
            : isValidQuery(isValid),
              synonyms(std::move(syns)),
              select(std::move(sel)),
              conditions(std::move(cons)) {}
};

}  // namespace spa

#endif  // SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
