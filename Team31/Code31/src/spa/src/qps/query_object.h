#ifndef SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
#define SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_

#include "common/aliases.h"
#include "qps/conditions/condition_clause.h"
#include "synonym.h"

namespace spa {
struct QueryObject {
    std::vector<const Synonym *> selected;
    const VecUniquePtr<Synonym> synonyms;
    const VecUniquePtr<ConditionClause> clauses;
    QueryObject(std::vector<const Synonym *> sel, VecUniquePtr<Synonym> syns,
                VecUniquePtr<ConditionClause> cons) noexcept
            : selected(std::move(sel)),
              synonyms(std::move(syns)),
              clauses(std::move(cons)) {}
};

}  // namespace spa

#endif  // SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
