#ifndef SRC_SPA_SRC_SOURCE_VALIDATOR_H_
#define SRC_SPA_SRC_SOURCE_VALIDATOR_H_

#include <array>
#include <filesystem>
#include <functional>
#include <memory>
#include <vector>

#include "common/tokenizer.h"
#include "token.h"

namespace spa {
class Validator {
  public:
    explicit Validator(const std::filesystem::path &filepath);
    std::unique_ptr<std::vector<Token>> Validate();

  private:
    static constexpr char kZero = '0';
    static constexpr std::array<SourceTokenType, 5> kArithmeticOpr = {
            SourceTokenType::kOperatorPlus, SourceTokenType::kOperatorMinus,
            SourceTokenType::kOperatorTimes, SourceTokenType::kOperatorDivide,
            SourceTokenType::kOperatorModulo};
    static constexpr std::array<SourceTokenType, 6> kRelationalOpr = {
            SourceTokenType::kRelLt, SourceTokenType::kRelLeq,
            SourceTokenType::kRelEq, SourceTokenType::kRelNeq,
            SourceTokenType::kRelGt, SourceTokenType::kRelGeq};
    enum CondExprSubTypes {
        kInvalid,
        kArithmetic,
        kSingular,
        kBracketed,
    };

    Tokenizer tokenizer_;
    std::unique_ptr<std::vector<Token>> tokens_;
    std::string curr_token_;

    bool Program();
    bool Procedure();
    bool StmtLst();
    bool NameSemiColon();
    bool While();
    bool If();
    bool Assign();
    /**
     * Returns true if curr_token_ is a valid constant
     * Assumes that curr_token_ is not empty and if first char is a digit then
     * the rest of the char are also digits.
     */
    bool IsConstant() const;
    void FetchToken();
    bool Accept(SourceTokenType type);
    bool Stmt();
    bool ArithmeticExpr(bool has_left = false);
    bool Group();
    bool CondExpr();
    CondExprSubTypes CondPrefix();
    CondExprSubTypes RelationalExpr();
    bool CondInfix();
    bool RelInfix();
    bool ArithOpr();
    bool RelOpr();
    bool VarConst();
    bool Condition();
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_VALIDATOR_H_
