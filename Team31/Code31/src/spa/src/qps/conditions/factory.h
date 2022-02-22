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
  private:
    Relationship rel_ = Relationship::kNone;
    bool is_transitive = false;
    int first_int_ = 0;
    int second_int_ = 0;
    std::string first_ident_;
    std::string second_ident_;
    Synonym* assign_ = nullptr;
    Synonym* first_syn_ = nullptr;
    Synonym* second_syn_ = nullptr;
    std::vector<QueryToken> second_exprs_;

  public:
    void SetRelationship(QueryTokenType type);
    void SetFirst(int first);
    void SetFirst(Synonym* syn);
    void SetFirst(const std::string& value);
    void SetSecond(int second);
    void SetSecond(Synonym* syn);
    void SetSecond(const std::string& value);
    void SetSecond(std::vector<QueryToken>& expr);
    void SetTrans(bool isTrans);
    void SetAssign(Synonym* syn);

    std::unique_ptr<ConditionClause> Build();
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_FACTORY_H_
