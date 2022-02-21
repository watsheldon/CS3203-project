#include "query_object.h"

namespace spa {

const VecUniquePtr<ConditionClause>& QueryObject::GetClauses() const {
    return conditions;
}
const Synonym* QueryObject::GetSelect() const { return select.get(); }
bool QueryObject::Valid() const { return isValidQuery; }
}  // namespace spa
