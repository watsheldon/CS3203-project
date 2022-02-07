#ifndef SRC_SPA_SRC_SOURCE_VALIDATOR_H_
#define SRC_SPA_SRC_SOURCE_VALIDATOR_H_

#include "tokenizer.h"

#include <filesystem>
#include <memory>
#include <vector>

#include "token.h"

namespace spa {
class Validator {
  public:
    explicit Validator(const std::filesystem::path &filepath);
    std::shared_ptr<std::vector<Token>> Validate();

  private:
    static constexpr char kZero = '0';

    Tokenizer tokenizer_;
    std::shared_ptr<std::vector<Token>> tokens_;
    bool has_error_ = false;
    std::string curr_token_;

    void Program();
    void Procedure();
    void StmtLst();
    void Stmt(bool first = false);
    void Read();
    void Print();
    void Call();
    void While();
    void If();
    void Assign();
    void CondExpr();
    void Expr();
    void RelExpr();
    void Term();
    void Factor();
    /**
     * Returns true if curr_token_ is a valid constant
     * Assumes that curr_token_ is not empty and if first char is a digit then
     * the rest of the char are also digits.
     */
    bool IsConstant() const;
    bool accept(SourceTokenType type);
    bool expect(SourceTokenType type);
    void fetchToken();
};
}

#endif //SRC_SPA_SRC_SOURCE_VALIDATOR_H_
