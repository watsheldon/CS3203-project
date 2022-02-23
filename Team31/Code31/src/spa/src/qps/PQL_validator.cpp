#include "PQL_validator.h"

#include <string_view>
#include <vector>

#include "query_token.h"

namespace spa {

std::vector<QueryToken> PQLValidator::Validate(std::string_view value) {
    tokens_.clear();
    tokenizer_(value);
    fetchToken();
    bool valid_query = Query();
    return valid_query ? std::move(tokens_) : std::vector<QueryToken>();
}

bool PQLValidator::Query() {
    if (!parseDeclaration()) return false;
    while (parseDeclaration()) {
    }
    if (!parseSelect()) return false;
    parseSuchThat();
    parsePattern();
    return curr_token_.empty();
}

bool PQLValidator::IsConstant() {
    if (!std::isdigit(curr_token_[0])) return false;
    return curr_token_.length() == 1 || curr_token_[0] != kZero;
}

void PQLValidator::fetchToken() { tokenizer_ >> curr_token_; }

bool PQLValidator::accept(QueryTokenType type) {
    if (curr_token_.empty()) return false;
    if (type < QueryTokenType::kWord) {
        auto target = Keyword(type);
        if (curr_token_ != target) return false;
        tokens_.emplace_back(type);
        fetchToken();
        return true;
    }
    if (type == QueryTokenType::kWord) {
        if (!std::isalpha(curr_token_[0])) return false;
        tokens_.emplace_back(type, curr_token_);
        fetchToken();
        return true;
    }
    if (IsConstant()) {
        tokens_.emplace_back(type, curr_token_);
        fetchToken();
        return true;
    }
    return false;
}

bool PQLValidator::expect(QueryTokenType type) {
    if (!accept(type)) {
        return false;
    }
    return true;
}

bool PQLValidator::parseDeclaration() {
    if (accept(QueryTokenType::kDeclStmt)) {
        return parseMultipleSynonym();
    }
    if (accept(QueryTokenType::kDeclRead)) {
        return parseMultipleSynonym();
    }
    if (accept(QueryTokenType::kDeclPrint)) {
        return parseMultipleSynonym();
    }
    if (accept(QueryTokenType::kDeclCall)) {
        return parseMultipleSynonym();
    }
    if (accept(QueryTokenType::kDeclWhile)) {
        return parseMultipleSynonym();
    }
    if (accept(QueryTokenType::kDeclIf)) {
        return parseMultipleSynonym();
    }
    if (accept(QueryTokenType::kDeclAssign)) {
        return parseMultipleSynonym();
    }
    if (accept(QueryTokenType::kDeclVariable)) {
        return parseMultipleSynonym();
    }
    if (accept(QueryTokenType::kDeclConstant)) {
        return parseMultipleSynonym();
    }
    if (accept(QueryTokenType::kDeclProcedure)) {
        return parseMultipleSynonym();
    }
    return false;
}

bool PQLValidator::parseMultipleSynonym() {
    if (!accept(QueryTokenType::kWord)) return false;
    while (!accept(QueryTokenType::kSemicolon)) {
        if (!accept(QueryTokenType::kComma)) return false;
        if (!accept(QueryTokenType::kWord)) return false;
    }
    return true;
}

bool PQLValidator::parseSelect() {
    if (!accept(QueryTokenType::kKeywordSelect)) return false;
    if (!accept(QueryTokenType::kWord)) return false;
    return true;
}

bool PQLValidator::parseSuchThat() {
    if (!accept(QueryTokenType::kKeywordSuch)) return false;
    if (!accept(QueryTokenType::kKeywordThat)) return false;
    if (accept(QueryTokenType::kKeywordFollows)) {
        accept(QueryTokenType::kOperatorTimes);
        return parseFollows();
    }
    if (accept(QueryTokenType::kKeywordParent)) {
        accept(QueryTokenType::kOperatorTimes);
        return parseParent();
    }
    if (accept(QueryTokenType::kKeywordUses)) {
        return parseUses();
    }
    if (accept(QueryTokenType::kKeywordModifies)) {
        return parseModifies();
    }
    return false;
}

bool PQLValidator::parseFollows() {
    return expect(QueryTokenType::kBracketL) && parseStmtRef() &&
           expect(QueryTokenType::kComma) && parseStmtRef() &&
           expect(QueryTokenType::kBracketR);
}

bool PQLValidator::parseParent() {
    return expect(QueryTokenType::kBracketL) && parseStmtRef() &&
           expect(QueryTokenType::kComma) && parseStmtRef() &&
           expect(QueryTokenType::kBracketR);
}

bool PQLValidator::parseUses() {
    return expect(QueryTokenType::kBracketL) && parseUsesModifiesStmtRef() &&
           expect(QueryTokenType::kComma) && parseEntRef() &&
           expect(QueryTokenType::kBracketR);
}
bool PQLValidator::parseModifies() {
    return expect(QueryTokenType::kBracketL) && parseUsesModifiesStmtRef() &&
           expect(QueryTokenType::kComma) && parseEntRef() &&
           expect(QueryTokenType::kBracketR);
}

// The first argument for Modifies and Uses cannot be UNDERSCORE
// as it is unclear whether _ refers to a statement or procedure
bool PQLValidator::parseUsesModifiesStmtRef() {
    return expect(QueryTokenType::kWord) || accept(QueryTokenType::kInteger);
}
bool PQLValidator::parsePattern() {
    if (!accept(QueryTokenType::kKeywordPattern)) return false;
    if (!accept(QueryTokenType::kWord)) return false;
    if (!accept(QueryTokenType::kBracketL)) return false;
    if (!parseEntRef()) return false;
    if (!accept(QueryTokenType::kComma)) return false;
    parseExpressionSpec();
    if (!accept(QueryTokenType::kBracketR)) return false;
    return true;
}
bool PQLValidator::parseStmtRef() {
    return accept(QueryTokenType::kWord) ||
           accept(QueryTokenType::kUnderscore) ||
           accept(QueryTokenType::kInteger);
}

bool PQLValidator::parseEntRef() {
    return accept(QueryTokenType::kWord) ||
           accept(QueryTokenType::kUnderscore) || parseIdentifier();
}

bool PQLValidator::parseIdentifier() {
    return expect(QueryTokenType::kQuote) && expect(QueryTokenType::kWord) &&
           expect(QueryTokenType::kQuote);
}
bool PQLValidator::parseExpressionSpec() {
    if (accept(QueryTokenType::kUnderscore)) {
        return expect(QueryTokenType::kQuote) && parseFactor() &&
               expect(QueryTokenType::kQuote) &&
               expect(QueryTokenType::kUnderscore);
    }
    return expect(QueryTokenType::kQuote) && parseFactor() &&
           expect(QueryTokenType::kQuote);
}
bool PQLValidator::parseFactor() {
    return accept(QueryTokenType::kInteger) || expect(QueryTokenType::kWord);
}

}  // namespace spa
