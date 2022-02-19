#include "query_object_builder.h"

#include <utility>

namespace spa {
QueryObjectBuilder& QueryObjectBuilder::withIsValid(bool isValid) {
    isValidQuery = isValid;
    return *this;
}
QueryObjectBuilder& QueryObjectBuilder::withHasSuchThatClause(bool hasSuchThat) {
    hasSuchThatClause = hasSuchThat;
    return *this;
}
QueryObjectBuilder& QueryObjectBuilder::withHasPattern(bool hasPattern) {
    hasPatternClause = hasPattern;
    return *this;
}
QueryObjectBuilder& QueryObjectBuilder::withSynonyms(
        std::vector<std::unique_ptr<Synonym>> syns) {
    synonyms = std::move(syns);
    return *this;
}
QueryObjectBuilder& QueryObjectBuilder::withSelect(
        std::unique_ptr<Synonym> sel) {
    select = std::move(sel);
    return *this;
}
QueryObjectBuilder& QueryObjectBuilder::withSuchThat(SuchThatClause suchthat) {
    such_that_ = std::move(suchthat);
    return *this;
}
QueryObjectBuilder& QueryObjectBuilder::withPattern(PatternClause pat) {
    pattern = std::move(pat);
    return *this;
}
QueryObject QueryObjectBuilder::build() {
    return QueryObject(isValidQuery, hasSuchThatClause, hasPatternClause,
                       synonyms, std::move(select), such_that_, pattern);
}
}  // namespace spa