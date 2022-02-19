#ifndef SPA_SRC_SPA_SRC_QPS_QUERY_OBJECT_BUILDER_H_
#define SPA_SRC_SPA_SRC_QPS_QUERY_OBJECT_BUILDER_H_

#include <utility>
#include <vector>

#include "declaration_cl.h"
#include "pattern_cl.h"
#include "query_object.h"
#include "select_cl.h"
#include "such_that_cl.h"
#include "synonym.h"

namespace spa {
class QueryObjectBuilder {
  private:
    bool isValidQuery;
    bool hasSuchThatClause;
    bool hasPatternClause;
    std::vector<std::unique_ptr<Synonym>> synonyms;
    std::unique_ptr<Synonym> select;
    SuchThatClause such_that_;
    PatternClause pattern;

  public:
    QueryObjectBuilder(bool isValid, bool hasSuchThat, bool hasPattern,
                       std::vector<std::unique_ptr<Synonym>> syns,
                       std::unique_ptr<Synonym> sel, SuchThatClause suchthat,
                       PatternClause pat)
            : isValidQuery(isValid),
              hasSuchThatClause(hasSuchThat),
              hasPatternClause(hasPattern),
              synonyms(std::move(syns)),
              select(std::move(sel)),
              such_that_(std::move(suchthat)),
              pattern(std::move(pat)) {}

    QueryObjectBuilder& withIsValid(bool isValid);
    QueryObjectBuilder& withHasSuchThatClause(bool hasSuchThat);
    QueryObjectBuilder& withHasPattern(bool hasPattern);
    QueryObjectBuilder& withSynonyms(
            std::vector<std::unique_ptr<Synonym>> syns);
    QueryObjectBuilder& withSelect(std::unique_ptr<Synonym> sel);
    QueryObjectBuilder& withSuchThat(SuchThatClause suchthat);
    QueryObjectBuilder& withPattern(PatternClause pat);
    QueryObject build();
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_QPS_QUERY_OBJECT_BUILDER_H_
