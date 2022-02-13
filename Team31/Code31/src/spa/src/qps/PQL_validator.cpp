#include <memory>
#include <vector>

#include "PQL_validator.h"
#include "query_token.h"


namespace spa {
PQLValidator::PQLValidator(const std::filesystem::path &filepath)
        : tokenizer_(filepath),
          tokens_(std::make_shared<std::vector<QueryToken>>()) {}


QueryObject PQLValidator::Validate() {
    std::vector<Declaration> declarations;
    QueryObject query_object(true, false, false, declarations,
            Select select, SuchThat such_that_, Pattern pattern);

    fetchToken();
}

bool PQLValidator::IsConstant() {
    if (!std::isdigit(curr_token_[0])) return false;
    return curr_token_.length() == 1 || curr_token_[0] != kZero;
}

void PQLValidator::fetchToken() {
    tokenizer_ >> curr_token_;
}

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
bool PQLValidator::Query() {

}
bool PQLValidator::Declaration() {

}
bool PQLValidator::Select() {

}
bool PQLValidator::SuchThat() {

}
bool PQLValidator::Pattern() {

}
}  // namespace spa
