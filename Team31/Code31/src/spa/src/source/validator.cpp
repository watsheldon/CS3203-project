#include "validator.h"

#include <memory>

#include "token.h"

namespace spa {
Validator::Validator(const std::filesystem::path &filepath)
        : tokenizer_(filepath) {}
std::unique_ptr<std::vector<Token>> Validator::Validate() {
    tokens_ = std::make_unique<std::vector<Token>>();
    fetchToken();
    bool valid_program = Program();
    return valid_program ? std::move(tokens_) : nullptr;
}
bool Validator::Procedure() {
    return expect(SourceTokenType::kName) &&    // proc_name
           expect(SourceTokenType::kBraceL) &&  // {
           StmtLst() &&                         //
           expect(SourceTokenType::kBraceR);    // }
}
bool Validator::expect(SourceTokenType type) {
    if (!accept(type)) {
        return false;
    }
    return true;
}
bool Validator::Program() {
    bool valid = false;
    while (accept(SourceTokenType::kKeywordProcedure)) {
        valid = Procedure();
    }
    return valid;
}
inline void Validator::fetchToken() { tokenizer_ >> curr_token_; }
bool Validator::accept(SourceTokenType type) {
    if (curr_token_.empty()) return false;
    if (type < SourceTokenType::kName) {
        auto target = GetSourceKeyword(type);
        if (curr_token_ != target) return false;
        tokens_->emplace_back(type);
        fetchToken();
        return true;
    }
    if (type == SourceTokenType::kName) {
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
bool Validator::StmtLst() {
    bool valid = false;
    while (Stmt()) {
        valid = true;
    }
    return valid;
}
bool Validator::Stmt() {
    if (StringToSourceType.at(tokenizer_.Peek()) ==
        SourceTokenType::kAssignEqual) {
        return expect(SourceTokenType::kName) && Assign();
    }
    if (accept(SourceTokenType::kKeywordRead)) {
        return Read();
    }
    if (accept(SourceTokenType::kKeywordPrint)) {
        return Print();
    }
    if (accept(SourceTokenType::kKeywordCall)) {
        return Call();
    }
    if (accept(SourceTokenType::kKeywordWhile)) {
        return While();
    }
    if (accept(SourceTokenType::kKeywordIf)) {
        return If();
    }
    return false;
}
bool Validator::Read() {
    return expect(SourceTokenType::kName)  // var_name
           && expect(SourceTokenType::kSemicolon);
}
bool Validator::Print() {
    return expect(SourceTokenType::kName)  // var_name
           && expect(SourceTokenType::kSemicolon);
}
bool Validator::Call() {
    return expect(SourceTokenType::kName)  // proc_name
           && expect(SourceTokenType::kSemicolon);
}
bool Validator::While() {
    return expect(SourceTokenType::kBracketL) &&  // (
           CondExpr() &&                          // cond_expr
           expect(SourceTokenType::kBracketR) &&  // )
           expect(SourceTokenType::kBraceL) &&    // {
           StmtLst() &&                           // stmtLst
           expect(SourceTokenType::kBraceR);      // }
}
bool Validator::If() {
    return expect(SourceTokenType::kBracketL) &&     // (
           CondExpr() &&                             // cond_expr
           expect(SourceTokenType::kBracketR) &&     // )
           expect(SourceTokenType::kKeywordThen) &&  // then
           expect(SourceTokenType::kBraceL) &&       // {
           StmtLst() &&                              // stmtLst
           expect(SourceTokenType::kBraceR) &&       // }
           expect(SourceTokenType::kKeywordElse) &&  // else
           expect(SourceTokenType::kBraceL) &&       // {
           StmtLst() &&                              // stmtLst
           expect(SourceTokenType::kBraceR);         // }
}
bool Validator::Assign() {
    return expect(SourceTokenType::kAssignEqual) &&  // =
           Expr() &&                                 // expr
           expect(SourceTokenType::kSemicolon);      // ;
}
bool Validator::CondExpr() {
    if (accept(SourceTokenType::kCondNot)) {
        return expect(SourceTokenType::kBracketL) &&  // (
               CondExpr() &&                          // cond_expr
               expect(SourceTokenType::kBracketR);    // )
    }
    if (accept(SourceTokenType::kBracketL)) {
        if (!(CondExpr() &&  // cond_expr
              expect(SourceTokenType::kBracketR))) {
            return false;
        }
        if (accept(SourceTokenType::kCondAnd) ||
            accept(SourceTokenType::kCondOr)) {
            return expect(SourceTokenType::kBracketL) &&  // (
                   CondExpr() &&                          // cond_expr
                   expect(SourceTokenType::kBracketR);    // )
        }
        return false;
    }
    return RelExpr();
}
bool Validator::RelExpr() {
    return Expr() &&                              // rel_factor
           (accept(SourceTokenType::kRelGt) ||    // >
            accept(SourceTokenType::kRelGeq) ||   // >=
            accept(SourceTokenType::kRelLt) ||    // <
            accept(SourceTokenType::kRelLeq) ||   // <=
            accept(SourceTokenType::kRelEq) ||    // ==
            accept(SourceTokenType::kRelNeq)) &&  // !=
           Expr();                                // rel_factor
}
bool Validator::Factor() {
    if (accept(SourceTokenType::kName) || accept(SourceTokenType::kInteger)) {
        return true;
    }
    if (accept(SourceTokenType::kBracketL)) {
        return Expr() && expect(SourceTokenType::kBracketR);
    }
    return false;
}
bool Validator::Term() {
    if (!Factor()) return false;
    while (accept(SourceTokenType::kOperatorTimes) ||
           accept(SourceTokenType::kOperatorDivide) ||
           accept(SourceTokenType::kOperatorModulo)) {
        Factor();
    }
    return true;
}
bool Validator::Expr() {
    if (!Term()) return false;
    while (accept(SourceTokenType::kOperatorPlus) ||
           accept(SourceTokenType::kOperatorMinus)) {
        Term();
    }
    return true;
}
}  // namespace spa
