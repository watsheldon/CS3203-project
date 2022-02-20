#ifndef SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
#define SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_

#include <vector>

#include "pattern_cl.h"
#include "select_cl.h"
#include "such_that_cl.h"
#include "synonym.h"

namespace spa {
class QueryObject {
  public:
    const bool isValidQuery;
    const std::vector<std::unique_ptr<Synonym>> synonyms;
    const std::unique_ptr<Synonym> select;
    const std::unique_ptr<SuchThatClause> such_that_;
    const std::unique_ptr<PatternClause> pattern;
    QueryObject(bool isValid, std::vector<std::unique_ptr<Synonym>> syns,
                std::unique_ptr<Synonym> sel,
                std::unique_ptr<SuchThatClause> suchthat,
                std::unique_ptr<PatternClause> pat)
            : isValidQuery(isValid),
              synonyms(std::move(syns)),
              select(std::move(sel)),
              such_that_(std::move(suchthat)),
              pattern(std::move(pat)) {}
};
}  // namespace spa

#endif  // SPA_TEAM31_CODE31_SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
