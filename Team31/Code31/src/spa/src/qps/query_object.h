#ifndef SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
#define SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_

#include "common/aliases.h"
#include "qps/conditions/condition_clause.h"
#include "synonym.h"

namespace spa {
struct QueryObject {
    const Synonym *const select;
    const VecUniquePtr<Synonym> synonyms;
    const VecUniquePtr<ConditionClause> clauses;
    QueryObject(const Synonym *sel, VecUniquePtr<Synonym> &&syns,
                VecUniquePtr<ConditionClause> &&cons)
            : select(sel),
              synonyms(std::move(syns)),
              clauses(std::move(cons)) {}
};

}  // namespace spa

#endif  // SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
