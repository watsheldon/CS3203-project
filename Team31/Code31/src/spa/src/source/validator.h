#ifndef SRC_SPA_SRC_SOURCE_VALIDATOR_H_
#define SRC_SPA_SRC_SOURCE_VALIDATOR_H_

#include <filesystem>
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

    Tokenizer tokenizer_;
    std::unique_ptr<std::vector<Token>> tokens_;
    std::string curr_token_;

    bool Program();
    bool Procedure();
    bool StmtLst();
    bool Read();
    bool Print();
    bool Call();
    bool While();
    bool If();
    bool Assign();
    bool CondExpr();
    bool Expr();
    bool RelExpr();
    bool Term();
    bool Factor();
    /**
     * Returns true if curr_token_ is a valid constant
     * Assumes that curr_token_ is not empty and if first char is a digit then
     * the rest of the char are also digits.
     */
    bool IsConstant() const;
    bool accept(SourceTokenType type);
    bool expect(SourceTokenType type);
    void fetchToken();
    bool Stmt();
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_VALIDATOR_H_
