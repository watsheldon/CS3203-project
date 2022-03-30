#ifndef SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
#define SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_

#include <utility>

#include "common/aliases.h"
#include "qps/conditions/condition_clause.h"
#include "synonym.h"
#include "synonym_with_attr.h"

namespace spa {
struct QueryObject {
    std::vector<SynonymWithAttr> selected;
    const VecUniquePtr<Synonym> synonyms;
    const VecUniquePtr<ConditionClause> clauses;
    bool valid;
    QueryObject(std::vector<SynonymWithAttr> sel, VecUniquePtr<Synonym> syns,
                VecUniquePtr<ConditionClause> cons) noexcept
            : selected(std::move(sel)),
              synonyms(std::move(syns)),
              clauses(std::move(cons)),
              valid(true) {}
    explicit QueryObject(std::vector<SynonymWithAttr> sel)
            : selected(std::move(sel)), valid(false) {}
};

}  // namespace spa

#endif  // SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
