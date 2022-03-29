#include "PQL_validator.h"

#include <algorithm>
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
        auto target = GetQueryKeyword(type);
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
    if (!Tuple()) return false;
    bool valid = true;
    while (valid) {
        if (Accept(QueryTokenType::kKeywordSuch)) {
            if (!Accept(QueryTokenType::kKeywordThat)) return false;
            valid = SuchThat();
        } else if (Accept(QueryTokenType::kKeywordPattern)) {
            valid = PatternCond();
        } else if (Accept(QueryTokenType::kKeywordWith)) {
            valid = AttrCond();
        } else {
            break;
        }
    }
    return valid && curr_token_.empty();
}
bool PQLValidator::Tuple() {
    return Accept(QueryTokenType::kAngleBracketL) ? MultipleElem() : Elem();
}
bool PQLValidator::MultipleElem() {
    bool valid;
    do {
        valid = Elem();
    } while (valid && Accept(QueryTokenType::kComma));
    return valid && Accept(QueryTokenType::kAngleBracketR);
}
bool PQLValidator::Elem() {
    if (!Accept(QueryTokenType::kWord)) return false;
    return !Accept(QueryTokenType::kDot) || AttrName();
}
bool PQLValidator::AttrName() {
    if (Accept(QueryTokenType::kDeclStmt))
        return Accept(QueryTokenType::kHashtag);
    return Accept(QueryTokenType::kAttrProc) ||
           Accept(QueryTokenType::kAttrValue) ||
           Accept(QueryTokenType::kAttrVar);
}
bool PQLValidator::SuchThat() {
    bool valid;
    do {
        valid = Relationship();
    } while (valid && Accept(QueryTokenType::kKeywordAnd));
    return valid;
}
bool PQLValidator::Relationship() {
    if (Accept(QueryTokenType::kKeywordFollows) ||
        Accept(QueryTokenType::kKeywordParent) ||
        Accept(QueryTokenType::kKeywordNext) ||
        Accept(QueryTokenType::kKeywordAffects)) {
        Accept(QueryTokenType::kOperatorTimes);
        return StmtStmt();
    }
    if (Accept(QueryTokenType::kKeywordUses) ||
        Accept(QueryTokenType::kKeywordModifies)) {
        return UsesModifies();
    }
    if (Accept(QueryTokenType::kKeywordCalls)) {
        Accept(QueryTokenType::kOperatorTimes);
        return Calls();
    }
    return false;
}
bool PQLValidator::StmtStmt() {
    return Accept(QueryTokenType::kBracketL) && StmtRef() &&
           Accept(QueryTokenType::kComma) && StmtRef() &&
           Accept(QueryTokenType::kBracketR);
}
bool PQLValidator::UsesModifies() {
    return Accept(QueryTokenType::kBracketL) && UsesModifiesStmtEntRef() &&
           Accept(QueryTokenType::kComma) && EntRef() &&
           Accept(QueryTokenType::kBracketR);
}
bool PQLValidator::Calls() {
    return Accept(QueryTokenType::kBracketL) && EntRef() &&
           Accept(QueryTokenType::kComma) && EntRef() &&
           Accept(QueryTokenType::kBracketR);
}
// The first argument for Modifies and Uses cannot be UNDERSCORE
// as it is unclear whether _ refers to a statement or procedure
bool PQLValidator::UsesModifiesStmtEntRef() {
    return Accept(QueryTokenType::kWord) || Accept(QueryTokenType::kInteger) ||
           Identifier();
}
bool PQLValidator::PatternCond() {
    bool valid;
    do {
        valid = Pattern();
    } while (valid && Accept(QueryTokenType::kKeywordAnd));
    return valid;
}
bool PQLValidator::Pattern() {
    bool valid_first_part;
    valid_first_part = Accept(QueryTokenType::kWord) &&
                       Accept(QueryTokenType::kBracketL) && EntRef() &&
                       Accept(QueryTokenType::kComma) && ExpressionSpec();
    if (!valid_first_part) return false;
    return Accept(QueryTokenType::kComma)
                   ? Accept(QueryTokenType::kUnderscore) &&
                             Accept(QueryTokenType::kBracketR)
                   : Accept(QueryTokenType::kBracketR);
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
bool PQLValidator::AttrCond() {
    bool valid;
    do {
        valid = AttrCompare();
    } while (valid && Accept(QueryTokenType::kKeywordAnd));
    return valid;
}
bool PQLValidator::AttrCompare() {
    return Ref() && Accept(QueryTokenType::kEqual) && Ref();
}
bool PQLValidator::Ref() {
    return Identifier() || Accept(QueryTokenType::kInteger) || AttrRef();
}
bool PQLValidator::AttrRef() {
    return Accept(QueryTokenType::kWord) && Accept(QueryTokenType::kDot) &&
           AttrName();
}
}  // namespace spa
