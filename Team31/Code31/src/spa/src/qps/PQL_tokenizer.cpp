#include "PQL_tokenizer.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <ios>
#include <string>
#include <string_view>

namespace spa {
// keep chars of token until pred is not met
void PQLTokenizer::KeepWhile(std::string &token, int (*pred)(int)) {
    auto last = std::find_if_not(token.begin() + 1, token.end(), pred);
    if (last == token.end()) return;
    KeepFirstOf(token, last - token.begin());
}
// keep first size len chars of the token
void PQLTokenizer::KeepFirstOf(std::string &token, long len) {
    assert(len <= token.length());
    if (token.length() == len) return;

    buffer_.seekg(len - (long)token.length(), std::ios_base::cur);
    token.resize(len);
}
void PQLTokenizer::KeepStmtNum(std::string &token) {
    return token[kStmt.length()] == kHashtag
                   ? KeepFirstOf(token, kStmt.length() + 1)
                   : KeepWhile(token, std::isalnum);
}
void PQLTokenizer::ExtractInto(std::string &token) {
    buffer_ >> token;
    if (!buffer_) error = true;
    if (error) {
        token.resize(0);
        return;
    }
    // special case for stmt#
    if (token.substr(0, kStmt.length()) == kStmt) {
        return KeepStmtNum(token);
    }
    // identifiers / keywords
    if (std::isalpha(token[0])) {
        return KeepWhile(token, std::isalnum);
    }
    // constants
    if (std::isdigit(token[0])) {
        return KeepWhile(token, std::isdigit);
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
