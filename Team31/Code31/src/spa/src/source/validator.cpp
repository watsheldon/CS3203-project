#include "validator.h"

#include <cassert>
#include <memory>

#include "token.h"

namespace spa {
Validator::Validator(const std::filesystem::path &filepath)
        : tokenizer_(filepath) {}
std::unique_ptr<std::vector<Token>> Validator::Validate() {
    tokens_ = std::make_unique<std::vector<Token>>();
    FetchToken();
    bool valid_program = Program();
    return valid_program ? std::move(tokens_) : nullptr;
}
bool Validator::Procedure() {
    return Accept(SourceTokenType::kName) &&  // proc_name
           StmtLst();
}
bool Validator::Program() {
    bool valid = false;
    while (Accept(SourceTokenType::kKeywordProcedure)) {
        valid = Procedure();
    }
    return valid;
}
inline void Validator::FetchToken() { tokenizer_ >> curr_token_; }
bool Validator::Accept(SourceTokenType type) {
    if (curr_token_.empty()) return false;
    if (type < SourceTokenType::kName) {
        auto target = GetSourceKeyword(type);
        if (curr_token_ != target) return false;
        tokens_->emplace_back(type);
        FetchToken();
        return true;
    }
    if (type == SourceTokenType::kName) {
        if (!std::isalpha(curr_token_[0])) return false;
        tokens_->emplace_back(type, curr_token_);
        FetchToken();
        return true;
    }
    if (IsConstant()) {
        tokens_->emplace_back(type, curr_token_);
        FetchToken();
        return true;
    }
    return false;
}
bool Validator::IsConstant() const {
    if (!std::isdigit(curr_token_[0])) return false;
    return curr_token_.length() == 1 || curr_token_[0] != kZero;
}
bool Validator::StmtLst() {
    if (!Accept(SourceTokenType::kBraceL) || !Stmt()) return false;
    while (Stmt())
        ;  // get all other stmt
    Accept(SourceTokenType::kBraceR);
    return true;
}
bool Validator::Stmt() {
    if (StringToSourceType.at(tokenizer_.Peek()) ==
        SourceTokenType::kAssignEqual) {
        return Accept(SourceTokenType::kName) && Assign();
    }
    if (Accept(SourceTokenType::kKeywordRead) ||
        Accept(SourceTokenType::kKeywordPrint) ||
        Accept(SourceTokenType::kKeywordCall)) {
        return NameSemiColon();
    }
    if (Accept(SourceTokenType::kKeywordWhile)) {
        return While();
    }
    if (Accept(SourceTokenType::kKeywordIf)) {
        return If();
    }
    return false;
}
bool Validator::NameSemiColon() {
    return Accept(SourceTokenType::kName) &&
           Accept(SourceTokenType::kSemicolon);
}
bool Validator::While() { return Condition() && StmtLst(); }
bool Validator::If() {
    return Condition() &&  //
           Accept(SourceTokenType::kKeywordThen) && StmtLst() &&
           Accept(SourceTokenType::kKeywordElse) && StmtLst();
}
bool Validator::Condition() {
    return Accept(SourceTokenType::kBracketL) && CondExpr() &&
           Accept(SourceTokenType::kBracketR);
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
        return Condition() ? kSingular : kInvalid;
    }
    if (Accept(SourceTokenType::kBracketL)) {
        return CondGroup();
    }
    return kInvalid;
}
Validator::CondExprSubTypes Validator::CondGroup() {
    auto prefix_type = CondPrefix();
    if (prefix_type == kInvalid || prefix_type == kBracketed && !CondInfix()) {
        return kInvalid;
    }
    if (Accept(SourceTokenType::kBracketR))
        return prefix_type == kSingular ? kBracketed : prefix_type;
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
    return Condition();
}
bool Validator::ArithOpr() {
    return std::any_of(kArithmeticOpr.begin(), kArithmeticOpr.end(),
                       [this](SourceTokenType type) { return Accept(type); });
}
bool Validator::RelOpr() {
    return std::any_of(kRelationalOpr.begin(), kRelationalOpr.end(),
                       [this](SourceTokenType type) { return Accept(type); });
}
bool Validator::VarConst() {
    return Accept(SourceTokenType::kName) || Accept(SourceTokenType::kInteger);
}
}  // namespace spa
