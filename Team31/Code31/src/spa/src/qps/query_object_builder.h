#ifndef SPA_SRC_SPA_SRC_QPS_QUERY_OBJECT_BUILDER_H_
#define SPA_SRC_SPA_SRC_QPS_QUERY_OBJECT_BUILDER_H_

#include <utility>
#include <vector>

#include "conditions/condition_clause.h"
#include "query_object.h"
#include "select_cl.h"
#include "synonym.h"

namespace spa {
class QueryObjectBuilder {
  private:
    bool isValidQuery_ = true;
    std::vector<std::unique_ptr<Synonym>> synonyms_;
    std::unique_ptr<Synonym> select_;
    std::vector<std::unique_ptr<ConditionClause>> conditions_;

  public:
    QueryObjectBuilder& SetIsValid(bool isValid);
    QueryObjectBuilder& SetSynonyms(std::vector<std::unique_ptr<Synonym>> syns);
    QueryObjectBuilder& SetSelect(std::unique_ptr<Synonym> sel);
    QueryObjectBuilder& SetConditions(
            std::vector<std::unique_ptr<ConditionClause>>& cons);
    QueryObject build();
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_QPS_QUERY_OBJECT_BUILDER_H_
