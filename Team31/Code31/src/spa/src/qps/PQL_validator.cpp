#include "PQL_validator.h"

#include <vector>
#include <string_view>

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
    if (!parseSuchThat()) return false;
    if (!parsePattern()) return false;
    return true;
}

bool PQLValidator::IsConstant() {
    if (!std::isdigit(curr_token_[0])) return false;
    return curr_token_.length() == 1 || curr_token_[0] != kZero;
}

void PQLValidator::fetchToken() { tokenizer_ >> curr_token_; }

bool PQLValidator::accept(QueryTokenType type) {
    if (curr_token_.empty()) return false;
    if (type < QueryTokenType::WORD) {
        auto target = Keyword(type);
        if (curr_token_ != target) return false;
        tokens_.emplace_back(type);
        fetchToken();
        return true;
    }
    if (type == QueryTokenType::WORD) {
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
    if (accept(QueryTokenType::STMT)) {
        return parseSynonym();
    }
    if (accept(QueryTokenType::READ)) {
        return parseSynonym();
    }
    if (accept(QueryTokenType::PRINT)) {
        return parseSynonym();
    }
    if (accept(QueryTokenType::CALL)) {
        return parseSynonym();
    }
    if (accept(QueryTokenType::WHILE)) {
        return parseSynonym();
    }
    if (accept(QueryTokenType::IF)) {
        return parseSynonym();
    }
    if (accept(QueryTokenType::ASSIGN)) {
        return parseSynonym();
    }
    if (accept(QueryTokenType::VARIABLE)) {
        return parseSynonym();
    }
    if (accept(QueryTokenType::CONSTANT)) {
        return parseSynonym();
    }
    if (accept(QueryTokenType::PROCEDURE)) {
        return parseSynonym();
    }
    return false;
}

bool PQLValidator::parseSynonym() {
    if (!accept(QueryTokenType::WORD)) return false;
    while (!accept(QueryTokenType::SEMICOLON)) {
        if (!accept(QueryTokenType::COMMA)) return false;
        if (!accept(QueryTokenType::WORD)) return false;
    }
    return true;
}

bool PQLValidator::parseSelect() {
    if (!accept(QueryTokenType::SELECT)) return false;
    if (!accept(QueryTokenType::WORD)) return false;
    return true;
}

bool PQLValidator::parseSuchThat() {
    if (!accept(QueryTokenType::SUCH)) return false;
    if (!accept(QueryTokenType::THAT)) return false;
    if (accept(QueryTokenType::FOLLOWS)) {
        accept(QueryTokenType::TIMES);
        return parseFollows();
    }
    if (accept(QueryTokenType::PARENT)) {
        accept(QueryTokenType::TIMES);
        return parseParent();
    }
    if (accept(QueryTokenType::USES)) {
        return parseUses();
    }
    if (accept(QueryTokenType::MODIFIES)) {
        return parseModifies();
    }
    return false;
}

bool PQLValidator::parseFollows() {
    return expect(QueryTokenType::LEFTBRACKET) && parseStmtRef() &&
           expect(QueryTokenType::COMMA) && parseStmtRef() &&
           expect(QueryTokenType::RIGHTBRACKET);
}

bool PQLValidator::parseParent() {
    return expect(QueryTokenType::LEFTBRACKET) && parseStmtRef() &&
           expect(QueryTokenType::COMMA) && parseStmtRef() &&
           expect(QueryTokenType::RIGHTBRACKET);
}

bool PQLValidator::parseUses() {
    return expect(QueryTokenType::LEFTBRACKET) && parseUsesModifiesStmtRef() &&
           expect(QueryTokenType::COMMA) && parseEntRef() &&
           expect(QueryTokenType::RIGHTBRACKET);
}
bool PQLValidator::parseModifies() {
    return expect(QueryTokenType::LEFTBRACKET) && parseUsesModifiesStmtRef() &&
           expect(QueryTokenType::COMMA) && parseEntRef() &&
           expect(QueryTokenType::RIGHTBRACKET);
}

// The first argument for Modifies and Uses cannot be UNDERSCORE
// as it is unclear whether _ refers to a statement or procedure
bool PQLValidator::parseUsesModifiesStmtRef() {
    return expect(QueryTokenType::WORD) || accept(QueryTokenType::INTEGER);
}
bool PQLValidator::parsePattern() {
    if (!accept(QueryTokenType::PATTERN)) return false;
    if (!accept(QueryTokenType::WORD)) return false;
    if (!accept(QueryTokenType::LEFTBRACKET)) return false;
    if (!parseEntRef()) return false;
    if (!accept(QueryTokenType::COMMA)) return false;
    parseExpressionSpec();
    if (!accept(QueryTokenType::RIGHTBRACKET)) return false;
    return true;
}
bool PQLValidator::parseStmtRef() {
    return parseSynonym() || expect(QueryTokenType::UNDERSCORE) ||
           accept(QueryTokenType::INTEGER);
}

bool PQLValidator::parseEntRef() {
    return parseSynonym() || expect(QueryTokenType::UNDERSCORE) ||
           parseIdentifier();
}

bool PQLValidator::parseIdentifier() {
    return expect(QueryTokenType::DOUBLEQUOTE) &&
           expect(QueryTokenType::WORD) && expect(QueryTokenType::DOUBLEQUOTE);
}
bool PQLValidator::parseExpressionSpec() {
    if (accept(QueryTokenType::UNDERSCORE)) {
        return expect(QueryTokenType::DOUBLEQUOTE) && parseFactor() &&
               expect(QueryTokenType::DOUBLEQUOTE) &&
               expect(QueryTokenType::UNDERSCORE);
    }
    return expect(QueryTokenType::DOUBLEQUOTE) && parseFactor() &&
           expect(QueryTokenType::DOUBLEQUOTE);
}
bool PQLValidator::parseFactor() {
    return accept(QueryTokenType::INTEGER) || expect(QueryTokenType::WORD);
}

}  // namespace spa
