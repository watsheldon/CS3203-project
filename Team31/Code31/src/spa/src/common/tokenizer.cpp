#include "tokenizer.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string_view>

namespace spa {
Tokenizer::Tokenizer(const std::filesystem::path &filepath) {
    std::ifstream source(filepath);
    buffer_ << source.rdbuf();
}
Tokenizer::Tokenizer(std::string_view str) {
    buffer_.str(str.data());
    error = false;
}
Tokenizer &Tokenizer::operator()(std::string_view str) {
    buffer_.clear();
    buffer_.str(str.data());
    error = false;
    return *this;
}
std::string Tokenizer::Next() {
    std::string token;
    if (peek_.empty()) {
        ExtractInto(token);
    } else {
        token = peek_;
        peek_.clear();
    }
    return token;
}
void Tokenizer::KeepWhile(std::string &token, int (*pred)(int)) {
    auto last = std::find_if_not(token.begin() + 1, token.end(), pred);
    if (last == token.end()) return;
    KeepFirstOf(token, last - token.begin());
}
void Tokenizer::KeepFirstOf(std::string &token, long len) {
    assert(len <= token.length());
    if (token.length() == len) return;

    buffer_.seekg(len - (long)token.length(), std::ios_base::cur);
    token.resize(len);
}
void Tokenizer::ExtractInto(std::string &token) {
    if (!(buffer_ >> token)) error = true;
    if (error) {
        token.resize(0);
        return;
    }

    // identifiers / keywords
    if (std::isalpha(token[0])) {
        return KeepWhile(token, std::isalnum);
    }
    // constants
    if (std::isdigit(token[0])) {
        return KeepWhile(token, std::isdigit);
    }
    if (std::strchr(kSpecialSingle, token[0])) {
        return KeepFirstOf(token, 1);
    }
    // For && and ||
    if (std::strchr(kSpecialPaired, token[0])) {
        if (token.length() == 1 || token[0] != token[1]) {
            token.resize(0);
            error = true;
            return;
        }
        return KeepFirstOf(token, 2);
    }
    // for ==, !=, <=, >=
    if (std::strchr(kSpecialMaybePaired, token[0])) {
        if (token.length() == 1) {
            return;
        }
        if (token[1] == '=') {
            return KeepFirstOf(token, 2);
        }
        return KeepFirstOf(token, 1);
    }

    // this should not be reached unless an illegal character is present
    error = true;
}
Tokenizer &Tokenizer::operator>>(std::string &token) {
    if (peek_.empty()) {
        ExtractInto(token);
    } else {
        token = peek_;
        peek_.clear();
    }
    return *this;
}
std::string Tokenizer::Peek() {
    if (peek_.empty()) {
        ExtractInto(peek_);
    }
    return peek_;
}
}  // namespace spa
