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

QueryObjectBuilder& QueryObjectBuilder::SetConditions(std::vector<std::unique_ptr<ConditionClause>>& cons) {
    conditions_ = std::move(cons);
    return *this;
}

QueryObject QueryObjectBuilder::build() {
    return QueryObject(isValidQuery_, std::move(synonyms_), std::move(select_),
                       std::move(conditions_));
}

}  // namespace spa
