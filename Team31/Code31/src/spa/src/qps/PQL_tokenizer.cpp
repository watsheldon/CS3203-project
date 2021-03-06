#include "PQL_tokenizer.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <ios>
#include <string>
#include <string_view>

namespace spa {
void PQLTokenizer::KeepDigit(std::string &token) {
    auto last = std::find_if_not(token.begin() + 1, token.end(),
                                 [](auto c) { return std::isdigit(c); });
    if (last == token.end()) return;
    KeepFirstOf(token, last - token.begin());
}
void PQLTokenizer::KeepAlnum(std::string &token) {
    auto last = std::find_if_not(token.begin() + 1, token.end(),
                                 [](auto c) { return std::isalnum(c); });
    if (last == token.end()) return;
    long keep_length = last - token.begin();
    if (token.substr(0, keep_length) == kStmt && *last == kHashtag)
        ++keep_length;
    KeepFirstOf(token, keep_length);
}
// keep first size len chars of the token
void PQLTokenizer::KeepFirstOf(std::string &token, long len) {
    assert(len <= token.length());
    if (token.length() == len) return;

    buffer_.seekg(len - (long)token.length(), std::ios_base::cur);
    token.resize(len);
}
void PQLTokenizer::ExtractInto(std::string &token) {
    buffer_ >> token;
    if (!buffer_) error = true;
    if (error) {
        token.resize(0);
        return;
    }
    // identifiers / keywords
    if (std::isalpha(token[0])) {
        return KeepAlnum(token);
    }
    // constants
    if (std::isdigit(token[0])) {
        return KeepDigit(token);
    }
    // operators
    if (std::strchr(kSpecial, token[0])) {
        return KeepFirstOf(token, 1);
    }
    // this should not be reached unless an illegal character is present
    error = true;
}
PQLTokenizer &PQLTokenizer::operator>>(std::string &token) {
    ExtractInto(token);
    return *this;
}
PQLTokenizer &PQLTokenizer::operator()(std::string_view str) {
    buffer_.clear();
    buffer_.str(str.data());
    error = false;
    return *this;
}
std::string PQLTokenizer::Next() {
    std::string token;
    ExtractInto(token);
    return token;
}

}  // namespace spa
