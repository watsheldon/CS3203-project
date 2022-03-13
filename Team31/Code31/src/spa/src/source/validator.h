#ifndef SRC_SPA_SRC_SOURCE_VALIDATOR_H_
#define SRC_SPA_SRC_SOURCE_VALIDATOR_H_

#include <filesystem>
#include <vector>

#include "common/tokenizer.h"
#include "token.h"

namespace spa {
class Validator {
  public:
    explicit Validator(const std::filesystem::path &filepath) noexcept;
    std::vector<Token> Validate() noexcept;

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
    std::vector<Token> tokens_;
    std::string curr_token_;

    bool Program() noexcept;
    bool Procedure() noexcept;
    bool StmtLst() noexcept;
    bool NameSemiColon() noexcept;
    bool While() noexcept;
    bool If() noexcept;
    bool Assign() noexcept;
    /**
     * Returns true if curr_token_ is a valid constant
     * Assumes that curr_token_ is not empty and if first char is a digit then
     * the rest of the char are also digits.
     */
    bool IsConstant() const noexcept;
    void FetchToken() noexcept;
    bool Accept(SourceTokenType type) noexcept;
    bool ArithmeticExpr(bool has_left = false) noexcept;
    bool Group() noexcept;
    bool CondExpr() noexcept;
    CondExprSubTypes CondPrefix() noexcept;
    CondExprSubTypes RelationalExpr() noexcept;
    bool CondInfix() noexcept;
    bool RelInfix() noexcept;
    bool ArithOpr() noexcept;
    bool RelOpr() noexcept;
    bool VarConst() noexcept;
    bool Condition() noexcept;
    CondExprSubTypes CondGroup() noexcept;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_VALIDATOR_H_
