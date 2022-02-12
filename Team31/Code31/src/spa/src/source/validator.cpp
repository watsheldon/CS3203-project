#include "validator.h"

#include <memory>

#include "token.h"

namespace spa {
Validator::Validator(const std::filesystem::path &filepath)
        : tokenizer_(filepath),
          tokens_(std::make_shared<std::vector<Token>>()) {}
std::shared_ptr<std::vector<Token>> Validator::Validate() {
    fetchToken();
    bool valid_program = Program();
    return valid_program ? tokens_ : nullptr;
}
bool Validator::Procedure() {
    return expect(kName) &&    // proc_name
           expect(kBraceL) &&  // {
           StmtLst() &&        //
           expect(kBraceR);    // }
}
bool Validator::expect(SourceTokenType type) {
    if (!accept(type)) {
        return false;
    }
    return true;
}
bool Validator::Program() {
    bool valid = false;
    while (accept(kKeywordProcedure)) {
        valid = Procedure();
    }
    return valid;
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
bool Validator::StmtLst() {
    bool valid = false;
    while (Stmt()) {
        valid = true;
    }
    return valid;
}
bool Validator::Stmt() {
    if (accept(kKeywordRead)) {
        return Read();
    }
    if (accept(kKeywordPrint)) {
        return Print();
    }
    if (accept(kKeywordCall)) {
        return Call();
    }
    if (accept(kKeywordWhile)) {
        return While();
    }
    if (accept(kKeywordIf)) {
        return If();
    }
    if (accept(kName)) {
        return Assign();
    }
    return false;
}
bool Validator::Read() {
    return expect(kName)  // var_name
           && expect(kSemicolon);
}
bool Validator::Print() {
    return expect(kName)  // var_name
           && expect(kSemicolon);
}
bool Validator::Call() {
    return expect(kName)  // proc_name
           && expect(kSemicolon);
}
bool Validator::While() {
    return expect(kBracketL) &&  // (
           CondExpr() &&         // cond_expr
           expect(kBracketR) &&  // )
           expect(kBraceL) &&    // {
           StmtLst() &&          // stmtLst
           expect(kBraceR);      // }
}
bool Validator::If() {
    return expect(kBracketL) &&     // (
           CondExpr() &&            // cond_expr
           expect(kBracketR) &&     // )
           expect(kKeywordThen) &&  // then
           expect(kBraceL) &&       // {
           StmtLst() &&             // stmtLst
           expect(kBraceR) &&       // }
           expect(kKeywordElse) &&  // else
           expect(kBraceL) &&       // {
           StmtLst() &&             // stmtLst
           expect(kBraceR);         // }
}
bool Validator::Assign() {
    return expect(kAssignEqual) &&  // =
           Expr() &&                // expr
           expect(kSemicolon);      // ;
}
bool Validator::CondExpr() {
    if (accept(kCondNot)) {
        return expect(kBracketL) &&  // (
               CondExpr() &&         // cond_expr
               expect(kBracketR);    // )
    }
    if (accept(kBracketL)) {
        if (!(CondExpr() &&  // cond_expr
              expect(kBracketR))) {
            return false;
        }
        if (accept(kCondAnd) || accept(kCondOr)) {
            return expect(kBracketL) &&  // (
                   CondExpr() &&         // cond_expr
                   expect(kBracketR);    // )
        }
        return false;
    }
    return RelExpr();
}
bool Validator::RelExpr() {
    return Expr() &&             // rel_factor
           (accept(kRelGt) ||    // >
            accept(kRelGeq) ||   // >=
            accept(kRelLt) ||    // <
            accept(kRelLeq) ||   // <=
            accept(kRelEq) ||    // ==
            accept(kRelNeq)) &&  // !=
           Expr();               // rel_factor
}
bool Validator::Factor() {
    if (accept(kName) || accept(kInteger)) {
        return true;
    }
    if (accept(kBracketL)) {
        return Expr() && expect(kBracketR);
    }
    return false;
}
bool Validator::Term() {
    if (!Factor()) return false;
    while (accept(kOperatorTimes) || accept(kOperatorDivide) ||
           accept(kOperatorModulo)) {
        Factor();
    }
    return true;
}
bool Validator::Expr() {
    if (!Term()) return false;
    while (accept(kOperatorPlus) || accept(kOperatorMinus)) {
        Term();
    }
    return true;
}
}  // namespace spa
