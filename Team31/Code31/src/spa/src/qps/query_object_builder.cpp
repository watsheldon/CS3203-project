#include "query_object_builder.h"

#include <utility>

namespace spa {
QueryObjectBuilder& QueryObjectBuilder::SetIsValid(bool isValid) {
    isValidQuery_ = isValid;
    return *this;
}

QueryObjectBuilder& QueryObjectBuilder::SetSynonyms(
        std::vector<Synonym* const>& syns) {
    synonyms_ = (std::move(syns));
    return *this;
}

QueryObjectBuilder& QueryObjectBuilder::SetSelect(Synonym* sel) {
    select_ = sel;
    return *this;
}

QueryObjectBuilder& QueryObjectBuilder::SetConditions(std::vector<std::unique_ptr<ConditionClause>>& cons) {
    conditions = std::move(cons);
    return *this;
}

QueryObject QueryObjectBuilder::build() {
    return {isValidQuery_, synonyms_, select_, conditions};
}


}  // namespace spa
