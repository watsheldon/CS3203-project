#ifndef SRC_SPA_SRC_QPS_QUERY_OBJECT_BUILDER_H_
#define SRC_SPA_SRC_QPS_QUERY_OBJECT_BUILDER_H_

#include <memory>
#include <utility>
#include <vector>

#include "conditions/condition_clause.h"
#include "query_object.h"
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
    std::unique_ptr<QueryObject> Build();
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_QUERY_OBJECT_BUILDER_H_
