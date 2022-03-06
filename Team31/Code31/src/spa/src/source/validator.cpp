#include "validator.h"

#include <cassert>
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
           ArithmeticExpr() &&                       // expr
           expect(SourceTokenType::kSemicolon);      // ;
}
bool Validator::ArithmeticExpr(bool has_left /*= false */) {
    if (accept(SourceTokenType::kBracketL)) {
        if (!Group()) return false;
    } else if (!VarConst()) {
        return false;
    }
    if (has_left) return true;
    while (ArithOpr()) {
        if (!ArithmeticExpr(true)) return false;
    }
    return true;
}
bool Validator::Group() {
    return ArithmeticExpr() && expect(SourceTokenType::kBracketR);
}
bool Validator::CondExpr() {
    auto prefix_type = CondPrefix();
    switch (prefix_type) {
        case kInvalid:
            return false;
        case kArithmetic:
            return RelInfix();
        case kSingular:
            return true;
        case kBracketed:
            return CondInfix();
        default:
            assert(false);
    }
}
Validator::CondExprSubTypes Validator::CondPrefix() {
    if (VarConst()) {
        return RelationalExpr();
    }
    if (accept(SourceTokenType::kCondNot)) {
        return WithBrackets([this] { return CondExpr(); }) ? kSingular
                                                           : kInvalid;
    }
    if (accept(SourceTokenType::kBracketL)) {
        auto prefix_type = CondPrefix();
        switch (prefix_type) {
            case kInvalid:
            case kBracketed:
                return kInvalid;
            case kArithmetic:
            case kSingular:
                if (!accept(SourceTokenType::kBracketR)) return kInvalid;
                return prefix_type == kArithmetic ? kArithmetic : kBracketed;
            default:
                assert(false);
        }
    }
    return kInvalid;
}
Validator::CondExprSubTypes Validator::RelationalExpr() {
    while (ArithOpr()) {
        if (!ArithmeticExpr(true)) {
            return kInvalid;
        }
    }
    if (RelOpr()) {
        if (!ArithmeticExpr()) {
            return kInvalid;
        }
        return kSingular;
    }
    return kArithmetic;
}
bool Validator::RelInfix() { return RelOpr() && ArithmeticExpr(); }
bool Validator::CondInfix() {
    if (!accept(SourceTokenType::kCondAnd) &&
        !accept(SourceTokenType::kCondOr)) {
        return false;
    }
    return expect(SourceTokenType::kBracketL) && CondExpr() &&
           expect(SourceTokenType::kBracketR);
}
bool Validator::AcceptAnyOf(const SourceTokenType *begin,
                            const SourceTokenType *end) {
    return std::any_of(begin, end,
                       [this](SourceTokenType type) { return accept(type); });
}
bool Validator::ArithOpr() {
    return AcceptAnyOf(kArithmeticOpr.begin(), kArithmeticOpr.end());
}
bool Validator::RelOpr() {
    return AcceptAnyOf(kRelationalOpr.begin(), kRelationalOpr.end());
}
bool Validator::VarConst() {
    return accept(SourceTokenType::kName) || accept(SourceTokenType::kInteger);
}
bool Validator::WithBrackets(const std::function<bool()> &body) {
    return accept(SourceTokenType::kBracketL) &&          // '('
           body() && accept(SourceTokenType::kBracketR);  // ')'
}
bool Validator::WithBraces(const std::function<bool()> &body) {
    return accept(SourceTokenType::kBraceL) &&          // '{'
           body() && accept(SourceTokenType::kBraceR);  // '}'
}
}  // namespace spa
