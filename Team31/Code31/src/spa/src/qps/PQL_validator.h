#ifndef SRC_SPA_SRC_QPS_PQL_VALIDATOR_H_
#define SRC_SPA_SRC_QPS_PQL_VALIDATOR_H_

#include <filesystem>
#include <string_view>

#include "PQL_tokenizer.h"
#include "query_object.h"
#include "query_token.h"
#include "select_cl.h"

namespace spa {
class PQLValidator {
  public:
    std::vector<QueryToken> Validate(std::string_view value);

  private:
    // a number of length greater than 1 starting with 0 is not a constant
    static constexpr char kZero = '0';

    PQLTokenizer tokenizer_;
    std::vector<QueryToken> tokens_;
    std::string curr_token_;

    bool Query();
    bool parseDeclaration();
    bool parseSelect();
    bool parseSuchThat();
    bool parsePattern();

    bool parseSynonym();
    bool parseStmtRef();
    bool parseUsesModifiesStmtRef();
    bool parseEntRef();
    bool parseExpressionSpec();
    bool parseIdentifier();
    bool parseFactor();

    bool parseFollows();
    bool parseParent();
    bool parseUses();
    bool parseModifies();
    bool IsConstant();
    void fetchToken();
    bool accept(QueryTokenType type);
    bool expect(QueryTokenType type);
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_PQL_VALIDATOR_H_
