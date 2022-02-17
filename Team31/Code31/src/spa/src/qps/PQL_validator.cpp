#include "PQL_validator.h"

#include <memory>
#include <vector>

#include "query_token.h"

namespace spa {
PQLValidator::PQLValidator(const std::filesystem::path &filepath)
        : tokenizer_(filepath),
          tokens_(std::make_shared<std::vector<QueryToken>>()) {}

std::shared_ptr<std::vector<QueryToken>> PQLValidator::Validate() {
    fetchToken();
    bool valid_query = Query();
    return valid_query ? tokens_ : nullptr;
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
        tokens_->emplace_back(type);
        fetchToken();
        return true;
    }
    if (type == QueryTokenType::WORD) {
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
        if(accept(QueryTokenType::TIMES)) {
            return parseFollowsT();
        }
        return parseFollows();
    }
    if (accept(QueryTokenType::PARENT)) {
        if(accept(QueryTokenType::TIMES)) {
            return parseParentT();
        }
        return parseParent();
    }
    if (accept(QueryTokenType::USES_S)) {
        return parseUsesS();
    }
    if (accept(QueryTokenType::MODIFIES_S)) {
        return parseModifiesS();
    }
    return false;
}

bool PQLValidator::parseFollows() {
    return expect(QueryTokenType::LEFTBRACKET) && parseStmtRef() &&
           expect(QueryTokenType::COMMA) && parseStmtRef() &&
           expect(QueryTokenType::RIGHTBRACKET);
}

bool PQLValidator::parseFollowsT() {
    return expect(QueryTokenType::LEFTBRACKET) && parseStmtRef() &&
           expect(QueryTokenType::COMMA) && parseStmtRef() &&
           expect(QueryTokenType::RIGHTBRACKET);
}

bool PQLValidator::parseParent() {
    return expect(QueryTokenType::LEFTBRACKET) && parseStmtRef() &&
           expect(QueryTokenType::COMMA) && parseStmtRef() &&
           expect(QueryTokenType::RIGHTBRACKET);
}
bool PQLValidator::parseParentT() {
    return expect(QueryTokenType::LEFTBRACKET) && parseStmtRef() &&
           expect(QueryTokenType::COMMA) && parseStmtRef() &&
           expect(QueryTokenType::RIGHTBRACKET);
}
bool PQLValidator::parseUsesS() {
    return expect(QueryTokenType::LEFTBRACKET) && parseStmtRef() &&
           expect(QueryTokenType::COMMA) && parseEntRef() &&
           expect(QueryTokenType::RIGHTBRACKET);
}
bool PQLValidator::parseModifiesS() {
    return expect(QueryTokenType::LEFTBRACKET) && parseStmtRef() &&
           expect(QueryTokenType::COMMA) && parseEntRef() &&
           expect(QueryTokenType::RIGHTBRACKET);
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
    return parseSynonym() || expect(QueryTokenType::UNDERSCORE) || IsConstant();
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
    return expect(QueryTokenType::UNDERSCORE) &&
           expect(QueryTokenType::DOUBLEQUOTE) && parseFactor() &&
           expect(QueryTokenType::DOUBLEQUOTE) &&
           expect(QueryTokenType::UNDERSCORE);
}
bool PQLValidator::parseFactor() {
    return IsConstant() || expect(QueryTokenType::WORD);
}

}  // namespace spa
