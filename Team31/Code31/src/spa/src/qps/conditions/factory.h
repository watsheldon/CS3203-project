#ifndef SRC_SPA_SRC_QPS_CONDITIONS_FACTORY_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_FACTORY_H_

#include <string>

#include "condition_clause.h"
#include "qps/query_token.h"
#include "qps/synonym.h"
namespace spa {
enum class Relationship {
    kNone,
    kFollows,
    kParent,
    kUses,
    kModifies,
    kPattern,
};

class Factory {
  public:
    void SetRelationship(QueryTokenType type);
    void SetFirst(int first);
    void SetFirst(Synonym* syn);
    void SetFirst(const std::string& value);
    void SetSecond(int second);
    void SetSecond(Synonym* syn);
    void SetSecond(const std::string& value);
    void SetSecond(std::vector<QueryToken>&& expr);
    void SetTransPartial();
    void SetAssign(Synonym* syn);

    std::unique_ptr<ConditionClause> Build();

  private:
    Relationship rel_;
    bool is_trans_partial_;
    int first_int_;
    int second_int_;
    std::string first_ident_;
    std::string second_ident_;
    Synonym* assign_;
    Synonym* first_syn_;
    Synonym* second_syn_;
    std::vector<QueryToken> second_exprs_;

    void Reset();
    std::unique_ptr<ConditionClause> BuildParent();
    std::unique_ptr<ConditionClause> BuildFollows();
    std::unique_ptr<ConditionClause> BuildUses();
    std::unique_ptr<ConditionClause> BuildModifies();
    std::unique_ptr<ConditionClause> BuildPattern();
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_FACTORY_H_
