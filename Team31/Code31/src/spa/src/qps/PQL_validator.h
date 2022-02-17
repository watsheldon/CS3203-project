#ifndef SPA_SRC_SPA_SRC_QPS_PQL_VALIDATOR_H_
#define SPA_SRC_SPA_SRC_QPS_PQL_VALIDATOR_H_

#include <filesystem>

#include "PQL_tokenizer.h"
#include "declaration_cl.h"
#include "pattern_cl.h"
#include "query_object.h"
#include "query_token.h"
#include "select_cl.h"
#include "such_that_cl.h"

namespace spa {
class PQLValidator {
  public:
    explicit PQLValidator(const std::filesystem::path &filepath);
    std::shared_ptr<std::vector<QueryToken>> Validate();

  private:
    // a number of length greater than 1 starting with 0 is not a constant
    static constexpr char kZero = '0';

    PQLTokenizer tokenizer_;
    std::shared_ptr<std::vector<QueryToken>> tokens_;
    std::string curr_token_;

    bool Query();
    bool parseDeclaration();
    bool parseSelect();
    bool parseSuchThat();
    bool parsePattern();

    bool parseSynonym();
    bool parseStmtRef();
    bool parseEntRef();
    bool parseExpressionSpec();
    bool parseIdentifier();
    bool parseFactor();

    bool parseFollows();
    bool parseFollowsT();
    bool parseParent();
    bool parseParentT();
    bool parseUsesS();
    bool parseModifiesS();
    bool IsConstant();
    //  bool IsIdentifier();
    void fetchToken();
    bool accept(QueryTokenType type);
    bool expect(QueryTokenType type);
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_PQL_VALIDATOR_H_