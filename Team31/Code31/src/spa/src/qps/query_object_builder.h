#ifndef SPA_SRC_SPA_SRC_QPS_QUERY_OBJECT_BUILDER_H_
#define SPA_SRC_SPA_SRC_QPS_QUERY_OBJECT_BUILDER_H_

#include <utility>
#include <vector>

#include "query_object.h"
#include "select_cl.h"
#include "such_that_cl.h"
#include "synonym.h"

namespace spa {
class QueryObjectBuilder {
  private:
    bool isValidQuery_ = true;
    std::vector<Synonym* const> synonyms_;
    Synonym* select_;
    std::vector<std::unique_ptr<ConditionClause>> conditions;

  public:
    QueryObjectBuilder& SetIsValid(bool isValid);
    QueryObjectBuilder& SetSynonyms(std::vector<Synonym* const>& syns);
    QueryObjectBuilder& SetSelect(Synonym* sel);
    QueryObjectBuilder& SetConditions(std::vector<std::unique_ptr<ConditionClause>>& cons);
    QueryObject build();
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_QPS_QUERY_OBJECT_BUILDER_H_
