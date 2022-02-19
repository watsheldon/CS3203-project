#include "query_object_builder.h"

#include <utility>

namespace spa {
QueryObjectBuilder& QueryObjectBuilder::SetIsValid(bool isValid) {
    isValidQuery_ = isValid;
    return *this;
}
QueryObjectBuilder& QueryObjectBuilder::SetSynonyms(
        std::vector<std::unique_ptr<Synonym>> syns) {
    synonyms_ = std::move(syns);
    return *this;
}
QueryObjectBuilder& QueryObjectBuilder::SetSelect(
        std::unique_ptr<Synonym> sel) {
    select_ = std::move(sel);
    return *this;
}
QueryObjectBuilder& QueryObjectBuilder::SetSuchThat(
        std::unique_ptr<SuchThatClause> suchthat) {
    such_that_ = std::move(suchthat);
    return *this;
}
QueryObjectBuilder& QueryObjectBuilder::SetPattern(
        std::unique_ptr<PatternClause> pat) {
    pattern_ = std::move(pat);
    return *this;
}
QueryObject QueryObjectBuilder::build() {
    return QueryObject(isValidQuery_, synonyms_, select_, such_that_, pattern_);
}

}  // namespace spa