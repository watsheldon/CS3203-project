#ifndef SRC_SPA_SRC_QPS_PQL_VALIDATOR_H_
#define SRC_SPA_SRC_QPS_PQL_VALIDATOR_H_

#include <array>
#include <string_view>

#include "PQL_tokenizer.h"
#include "query_object.h"
#include "query_token.h"

namespace spa {
class PQLValidator {
  public:
    std::vector<QueryToken> Validate(std::string_view value);

  private:
    // a number of length greater than 1 starting with 0 is not a constant
    static constexpr char kZero = '0';
    static constexpr const auto kDeclTypes = []() constexpr {
        std::array<QueryTokenType, 10> result{};
        for (int i = static_cast<int>(QueryTokenType::kDeclStmt);
             i < static_cast<int>(QueryTokenType::kKeywordSelect); i++) {
            result[i] = QueryTokenType{i};
        }
        return result;
    }
    ();
    static constexpr std::array<QueryTokenType, 5> kArithmeticOpr = {
            QueryTokenType::kOperatorPlus, QueryTokenType::kOperatorMinus,
            QueryTokenType::kOperatorTimes, QueryTokenType::kOperatorDivide,
            QueryTokenType::kOperatorModulo};
    PQLTokenizer tokenizer_;
    std::vector<QueryToken> tokens_;
    std::string curr_token_;

    bool Query();
    bool Declaration();
    bool Select();
    bool Tuple();
    bool Elem();
    bool MultipleElem();
    bool AttrName();
    bool SuchThat();
    bool Pattern();
    bool With();

    bool SynDecl();
    bool MultipleSynonym();
    bool StmtRef();
    bool UsesModifiesStmtEntRef();
    bool EntRef();
    bool ExpressionSpec();
    bool Identifier();
    bool Factor();
    bool ArithOpr();
    bool ArithmeticExpr(bool has_left = false);
    bool Group();

    bool RelRef();
    bool StmtStmt();
    bool UsesModifies();
    bool Calls();
    bool IsConstant();
    void FetchToken();
    bool Accept(QueryTokenType type);
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_PQL_VALIDATOR_H_
