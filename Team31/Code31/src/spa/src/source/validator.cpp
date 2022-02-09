#include "validator.h"

#include <memory>

#include "token.h"

namespace spa {
Validator::Validator(const std::filesystem::path &filepath)
        : tokenizer_(filepath),
          tokens_(std::make_shared<std::vector<Token>>()) {}
std::shared_ptr<std::vector<Token>> Validator::Validate() {
    Program();
    if (has_error_) return {};

    return tokens_;
}
void Validator::Procedure() {
    expect(kKeywordProcedure);
    expect(kName);  // proc_name
    expect(kBraceL);
    StmtLst();
    expect(kBraceR);
}
bool Validator::expect(SourceTokenType type) {
    if (!accept(type)) {
        has_error_ = true;
        return false;
    } else
        return true;
}
void Validator::Program() {
    fetchToken();
    while (curr_token_.length()) {
        Procedure();
    }
}
inline void Validator::fetchToken() { tokenizer_ >> curr_token_; }
bool Validator::accept(SourceTokenType type) {
    if (curr_token_.empty()) return false;
    if (type < kName) {
        auto target = Keyword(type);
        if (curr_token_ != target) return false;
        tokens_->emplace_back(type);
        fetchToken();
        return true;
    }
    if (type == kName) {
        if (!std::isalpha(curr_token_[0])) return false;
        tokens_->emplace_back(type, curr_token_);
        fetchToken();
        return true;
    }
    if (IsConstant()) {
        tokens_->emplace_back(type, curr_token_);
        fetchToken();
        return true;
    }
    return false;
}
bool Validator::IsConstant() const {
    if (!std::isdigit(curr_token_[0])) return false;
    return curr_token_.length() == 1 || curr_token_[0] != kZero;
}
void Validator::StmtLst() { Stmt(true); }
void Validator::Stmt(bool first) {
    if (accept(kKeywordRead)) {
        Read();
    } else if (accept(kKeywordPrint)) {
        Print();
    } else if (accept(kKeywordCall)) {
        Call();
    } else if (accept(kKeywordWhile)) {
        While();
    } else if (accept(kKeywordIf)) {
        If();
    } else if (accept(kName)) {
        Assign();
    } else if (first) {
        has_error_ = true;
        return;
    } else {
        return;
    }
    Stmt();
}
void Validator::Read() {
    expect(kName);
    expect(kSemicolon);
}
void Validator::Print() {
    expect(kName);
    expect(kSemicolon);
}
void Validator::Call() {
    expect(kName);
    expect(kSemicolon);
}
void Validator::While() {
    expect(kBracketL);
    CondExpr();
    expect(kBracketR);
    expect(kBraceL);
    StmtLst();
    expect(kBraceR);
}
void Validator::If() {
    expect(kBracketL);
    CondExpr();
    expect(kBracketR);
    expect(kKeywordThen);
    expect(kBraceL);
    StmtLst();
    expect(kBraceR);
    expect(kKeywordElse);
    expect(kBraceL);
    StmtLst();
    expect(kBraceR);
}
void Validator::Assign() {
    expect(kAssignEqual);
    Expr();
    expect(kSemicolon);
}
void Validator::CondExpr() {
    if (accept(kCondNot)) {
        expect(kBracketL);
        CondExpr();
        expect(kBracketR);
    } else if (accept(kBracketL)) {
        CondExpr();
        expect(kBracketR);
        if (accept(kCondAnd) || accept(kCondOr)) {
            expect(kBracketL);
            CondExpr();
            expect(kBracketR);
        } else {
            has_error_ = true;
            return;
        }
    } else {
        RelExpr();
    }
}
void Validator::RelExpr() {
    Expr();
    if (!(accept(kRelGt) || accept(kRelGeq) || accept(kRelLt) ||
          accept(kRelLeq) || accept(kRelEq) || accept(kRelNeq))) {
        has_error_ = true;
        return;
    }
    Expr();
}
void Validator::Factor() {
    if (accept(kName) || accept(kInteger)) {
        // pass
    } else if (accept(kBracketL)) {
        Expr();
        expect(kBracketR);
    } else {
        has_error_ = true;
        return;
    }
}
void Validator::Term() {
    Factor();
    while (accept(kOperatorTimes) || accept(kOperatorDivide) ||
           accept(kOperatorModulo)) {
        Factor();
    }
}
void Validator::Expr() {
    Term();
    while (accept(kOperatorPlus) || accept(kOperatorMinus)) {
        Term();
    }
}
}  // namespace spa
