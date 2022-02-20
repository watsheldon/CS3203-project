#ifndef SPA_SRC_SPA_SRC_QPS_QUERY_OBJECT_BUILDER_H_
#define SPA_SRC_SPA_SRC_QPS_QUERY_OBJECT_BUILDER_H_

#include <utility>
#include <vector>

#include "pattern_cl.h"
#include "query_object.h"
#include "select_cl.h"
#include "such_that_cl.h"
#include "synonym.h"

namespace spa {
class QueryObjectBuilder {
  private:
    bool isValidQuery_ = true;
    std::vector<std::unique_ptr<Synonym>> synonyms_;
    std::unique_ptr<Synonym> select_;
    std::unique_ptr<SuchThatClause> such_that_;
    std::unique_ptr<PatternClause> pattern_;

  public:
    QueryObjectBuilder& SetIsValid(bool isValid);
    QueryObjectBuilder& SetSynonyms(std::vector<std::unique_ptr<Synonym>> syns);
    QueryObjectBuilder& SetSelect(std::unique_ptr<Synonym> sel);
    QueryObjectBuilder& SetSuchThat(std::unique_ptr<SuchThatClause> suchthat);
    QueryObjectBuilder& SetPattern(std::unique_ptr<PatternClause> pat);
    QueryObject build();
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_QPS_QUERY_OBJECT_BUILDER_H_
