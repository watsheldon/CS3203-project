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
    return Accept(SourceTokenType::kName) &&  // proc_name
           WithBraces([this] { return StmtLst(); });
}
bool Validator::Program() {
    bool valid = false;
    while (Accept(SourceTokenType::kKeywordProcedure)) {
        valid = Procedure();
    }
    return valid;
}
inline void Validator::fetchToken() { tokenizer_ >> curr_token_; }
bool Validator::Accept(SourceTokenType type) {
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
        return Accept(SourceTokenType::kName) && Assign();
    }
    if (Accept(SourceTokenType::kKeywordRead)) {
        return Read();
    }
    if (Accept(SourceTokenType::kKeywordPrint)) {
        return Print();
    }
    if (Accept(SourceTokenType::kKeywordCall)) {
        return Call();
    }
    if (Accept(SourceTokenType::kKeywordWhile)) {
        return While();
    }
    if (Accept(SourceTokenType::kKeywordIf)) {
        return If();
    }
    return false;
}
bool Validator::Read() {
    return Accept(SourceTokenType::kName)  // var_name
           && Accept(SourceTokenType::kSemicolon);
}
bool Validator::Print() {
    return Accept(SourceTokenType::kName)  // var_name
           && Accept(SourceTokenType::kSemicolon);
}
bool Validator::Call() {
    return Accept(SourceTokenType::kName)  // proc_name
           && Accept(SourceTokenType::kSemicolon);
}
bool Validator::While() {
    return Accept(SourceTokenType::kBracketL) &&  // (
           CondExpr() &&                          // cond_expr
           Accept(SourceTokenType::kBracketR) &&  // )
           Accept(SourceTokenType::kBraceL) &&    // {
           StmtLst() &&                           // stmtLst
           Accept(SourceTokenType::kBraceR);      // }
}
bool Validator::If() {
    return Accept(SourceTokenType::kBracketL) &&     // (
           CondExpr() &&                             // cond_expr
           Accept(SourceTokenType::kBracketR) &&     // )
           Accept(SourceTokenType::kKeywordThen) &&  // then
           Accept(SourceTokenType::kBraceL) &&       // {
           StmtLst() &&                              // stmtLst
           Accept(SourceTokenType::kBraceR) &&       // }
           Accept(SourceTokenType::kKeywordElse) &&  // else
           Accept(SourceTokenType::kBraceL) &&       // {
           StmtLst() &&                              // stmtLst
           Accept(SourceTokenType::kBraceR);         // }
}
bool Validator::Assign() {
    return Accept(SourceTokenType::kAssignEqual) &&  // =
           ArithmeticExpr() &&                       // expr
           Accept(SourceTokenType::kSemicolon);      // ;
}
bool Validator::ArithmeticExpr(bool has_left /*= false */) {
    if (Accept(SourceTokenType::kBracketL)) {
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
    return ArithmeticExpr() && Accept(SourceTokenType::kBracketR);
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
    if (Accept(SourceTokenType::kCondNot)) {
        return WithBrackets([this] { return CondExpr(); }) ? kSingular
                                                           : kInvalid;
    }
    if (Accept(SourceTokenType::kBracketL)) {
        auto prefix_type = CondPrefix();
        switch (prefix_type) {
            case kInvalid:
            case kBracketed:
                return kInvalid;
            case kArithmetic:
            case kSingular:
                if (!Accept(SourceTokenType::kBracketR)) return kInvalid;
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
    if (!Accept(SourceTokenType::kCondAnd) &&
        !Accept(SourceTokenType::kCondOr)) {
        return false;
    }
    return Accept(SourceTokenType::kBracketL) && CondExpr() &&
           Accept(SourceTokenType::kBracketR);
}
bool Validator::AcceptAnyOf(const SourceTokenType *begin,
                            const SourceTokenType *end) {
    return std::any_of(begin, end,
                       [this](SourceTokenType type) { return Accept(type); });
}
bool Validator::ArithOpr() {
    return AcceptAnyOf(kArithmeticOpr.begin(), kArithmeticOpr.end());
}
bool Validator::RelOpr() {
    return AcceptAnyOf(kRelationalOpr.begin(), kRelationalOpr.end());
}
bool Validator::VarConst() {
    return Accept(SourceTokenType::kName) || Accept(SourceTokenType::kInteger);
}
bool Validator::WithBrackets(const std::function<bool()> &body) {
    return Accept(SourceTokenType::kBracketL) &&          // '('
           body() && Accept(SourceTokenType::kBracketR);  // ')'
}
bool Validator::WithBraces(const std::function<bool()> &body) {
    return Accept(SourceTokenType::kBraceL) &&          // '{'
           body() && Accept(SourceTokenType::kBraceR);  // '}'
}
}  // namespace spa
