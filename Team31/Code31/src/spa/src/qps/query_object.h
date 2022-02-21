#ifndef SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
#define SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_

#include <memory>
#include <vector>

#include "common/aliases.h"
#include "qps/conditions/condition_clause.h"
#include "synonym.h"

namespace spa {
class QueryObject {
  public:
    QueryObject(bool isValid, VecUniquePtr<Synonym> syns,
                std::unique_ptr<Synonym> sel,
                VecUniquePtr<ConditionClause> cons)
            : isValidQuery(isValid),
              synonyms(std::move(syns)),
              select(std::move(sel)),
              conditions(std::move(cons)) {}
    [[nodiscard]] const VecUniquePtr<ConditionClause>& GetClauses() const;
    [[nodiscard]] const Synonym* GetSelect() const;
    [[nodiscard]] bool Valid() const;

  private:
    const bool isValidQuery;
    const VecUniquePtr<Synonym> synonyms;
    const std::unique_ptr<Synonym> select;
    VecUniquePtr<ConditionClause> conditions;
};

}  // namespace spa

#endif  // SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
