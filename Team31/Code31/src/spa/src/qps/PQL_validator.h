#ifndef SPA_SRC_SPA_SRC_QPS_PQL_VALIDATOR_H_
#define SPA_SRC_SPA_SRC_QPS_PQL_VALIDATOR_H_

#include <filesystem>
#include "declaration.h"
#include "pattern.h"
#include "PQL_tokenizer.h"
#include "query_object.h"
#include "query_token.h"
#include "select.h"
#include "such_that.h"


namespace spa {
class PQLValidator{
  public:
    explicit PQLValidator(const std::filesystem::path &filepath);
    QueryObject Validate();

  private:
    // a number of length greater than 1 starting with 0 is not a constant
    static constexpr char kZero = '0';

    PQLTokenizer tokenizer_;
    std::shared_ptr<std::vector<QueryToken>> tokens_;
    std::string curr_token_;
    std::vector<Declaration> declarations;


    bool Query();
    bool Declaration();
    bool Select();
    bool SuchThat();
    bool Pattern();

    bool IsConstant();
    void fetchToken();
    bool accept(QueryTokenType type);
    bool expect(QueryTokenType type);
};
}

#endif  // SPA_SRC_SPA_SRC_QPS_PQL_VALIDATOR_H_
