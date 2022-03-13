#include "PQL_validator.h"

#include <string_view>
#include <vector>

#include "query_token.h"

namespace spa {

std::vector<QueryToken> PQLValidator::Validate(std::string_view value) {
    tokens_.clear();
    tokenizer_(value);
    FetchToken();
    bool valid_query = Query();
    return valid_query ? std::move(tokens_) : std::vector<QueryToken>();
}

bool PQLValidator::Query() { return Declaration() && Select(); }

bool PQLValidator::IsConstant() {
    if (!std::isdigit(curr_token_[0])) return false;
    return curr_token_.length() == 1 || curr_token_[0] != kZero;
}

void PQLValidator::FetchToken() { tokenizer_ >> curr_token_; }

bool PQLValidator::Accept(QueryTokenType type) {
    if (curr_token_.empty()) return false;
    if (type < QueryTokenType::kWord) {
        auto target = Keyword(type);
        if (curr_token_ != target) return false;
        tokens_.emplace_back(type);
        FetchToken();
        return true;
    }
    if (type == QueryTokenType::kWord) {
        if (!std::isalpha(curr_token_[0])) return false;
        tokens_.emplace_back(type, curr_token_);
        FetchToken();
        return true;
    }
    if (IsConstant()) {
        tokens_.emplace_back(type, curr_token_);
        FetchToken();
        return true;
    }
    return false;
}

bool PQLValidator::Declaration() {
    while (SynDecl()) {
        if (!MultipleSynonym()) return false;
    }
    return true;
}

bool PQLValidator::SynDecl() {
    return std::any_of(
            kDeclTypes.begin(), kDeclTypes.end(),
            [this](const QueryTokenType type) { return Accept(type); });
}

bool PQLValidator::MultipleSynonym() {
    if (!Accept(QueryTokenType::kWord)) return false;
    while (Accept(QueryTokenType::kComma)) {
        if (!Accept(QueryTokenType::kWord)) {
            return false;
        }
    }
    return Accept(QueryTokenType::kSemicolon);
}

bool PQLValidator::Select() {
    if (!Accept(QueryTokenType::kKeywordSelect)) return false;
    if (!Accept(QueryTokenType::kWord)) return false;
    bool valid = true;
    while (valid) {
        if (Accept(QueryTokenType::kKeywordSuch)) {
            valid = SuchThat();
        } else if (Accept(QueryTokenType::kKeywordPattern)) {
            valid = Pattern();
        } else {
            break;
        }
    }
    return valid && curr_token_.empty();
}

bool PQLValidator::SuchThat() {
    if (!Accept(QueryTokenType::kKeywordThat)) return false;
    if (Accept(QueryTokenType::kKeywordFollows)) {
        Accept(QueryTokenType::kOperatorTimes);
        return FollowsParent();
    }
    if (Accept(QueryTokenType::kKeywordParent)) {
        Accept(QueryTokenType::kOperatorTimes);
        return FollowsParent();
    }
    if (Accept(QueryTokenType::kKeywordUses)) {
        return UsesModifies();
    }
    if (Accept(QueryTokenType::kKeywordModifies)) {
        return UsesModifies();
    }
    return false;
}

bool PQLValidator::FollowsParent() {
    return Accept(QueryTokenType::kBracketL) && StmtRef() &&
           Accept(QueryTokenType::kComma) && StmtRef() &&
           Accept(QueryTokenType::kBracketR);
}

bool PQLValidator::UsesModifies() {
    return Accept(QueryTokenType::kBracketL) && UsesModifiesStmtEntRef() &&
           Accept(QueryTokenType::kComma) && EntRef() &&
           Accept(QueryTokenType::kBracketR);
}

// The first argument for Modifies and Uses cannot be UNDERSCORE
// as it is unclear whether _ refers to a statement or procedure
bool PQLValidator::UsesModifiesStmtEntRef() {
    return Accept(QueryTokenType::kWord) || Accept(QueryTokenType::kInteger);
}
bool PQLValidator::Pattern() {
    if (!Accept(QueryTokenType::kWord)) return false;
    if (!Accept(QueryTokenType::kBracketL)) return false;
    if (!EntRef()) return false;
    if (!Accept(QueryTokenType::kComma)) return false;
    ExpressionSpec();
    if (!Accept(QueryTokenType::kBracketR)) return false;
    return true;
}
bool PQLValidator::StmtRef() {
    return Accept(QueryTokenType::kWord) ||
           Accept(QueryTokenType::kUnderscore) ||
           Accept(QueryTokenType::kInteger);
}

bool PQLValidator::EntRef() {
    return Accept(QueryTokenType::kWord) ||
           Accept(QueryTokenType::kUnderscore) || Identifier();
}

bool PQLValidator::Identifier() {
    return Accept(QueryTokenType::kQuote) && Accept(QueryTokenType::kWord) &&
           Accept(QueryTokenType::kQuote);
}
bool PQLValidator::ExpressionSpec() {
    bool not_wild = true;
    if (Accept(QueryTokenType::kUnderscore)) {
        if (!Accept(QueryTokenType::kQuote)) return true;
        not_wild = false;
    } else if (!Accept(QueryTokenType::kQuote)) {
        return false;
    }
    return ArithmeticExpr() && Accept(QueryTokenType::kQuote) &&
           (not_wild || Accept(QueryTokenType::kUnderscore));
}

bool PQLValidator::ArithOpr() {
    return std::any_of(kArithmeticOpr.begin(), kArithmeticOpr.end(),
                       [this](QueryTokenType type) { return Accept(type); });
}

bool PQLValidator::ArithmeticExpr(bool has_left /*= false */) {
    if (Accept(QueryTokenType::kBracketL)) {
        if (!Group()) return false;
    } else if (!Factor()) {
        return false;
    }
    if (has_left) return true;
    while (ArithOpr()) {
        if (!ArithmeticExpr(true)) return false;
    }
    return true;
}
bool PQLValidator::Group() {
    return ArithmeticExpr() && Accept(QueryTokenType::kBracketR);
}

bool PQLValidator::Factor() {
    return Accept(QueryTokenType::kInteger) || Accept(QueryTokenType::kWord);
}
bool PQLValidator::With() {
    // todo : new condition clause
    return false;
}

}  // namespace spa
