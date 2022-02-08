#include "tokenizer.h"

#include <cctype>
#include <cstring>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace spa {
Tokenizer::Tokenizer(const std::filesystem::path &filepath) {
    std::ifstream source(filepath);
    buffer_ << source.rdbuf();
}
std::string Tokenizer::Next() {
    if (!buffer_)
        return {};

    std::string token;
    // token might contain extraneous characters caused by optional whitespace
    buffer_ >> token;

    // identifiers / keywords
    if (std::isalpha(token[0])) {
        auto last = std::find_if_not(token.begin() + 1, token.end(), isalnum);
        if (last == token.end())
            return token;

        // reverse the stream
        buffer_.seekg(last - token.end(), std::ios_base::cur);
        return token.substr(0, last - token.begin());
    }

    // constants
    if (std::isdigit(token[0])) {
        auto last = std::find_if_not(token.begin() + 1, token.end(), isdigit);
        if (last == token.end())
            return token;

        // reverse the stream
        buffer_.seekg(last - token.end(), std::ios_base::cur);
        return token.substr(0, last - token.begin());
    }

    if (std::strchr(kSpecialSingle, token[0])) {
        // either len 1 or followed by alnum
        if (token.length() == 1)
            return token;

        // reverse the stream
        buffer_.seekg(1 - (long) token.length(), std::ios_base::cur);
        return token.substr(0, 1);
    }

    if (std::strchr(kSpecialPaired, token[0])) {
        if (token.length() == 1)
            return {};

        if (token[0] == token[1]) {
            if (token.length() == 2)
                return token;

            // reverse the stream
            buffer_.seekg(2 - (long) token.length(), std::ios_base::cur);
            return token.substr(0, 2);
        }
        return {};
    }

    if (std::strchr(kSpecialMaybePaired, token[0])) {
        if (token.length() == 1 || std::isalnum(token[1]))
            return token.substr(0, 1);

        if (token[1] != '=')
            return {};

        if (token.length() == 2)
            return token;

        // reverse the stream
        buffer_.seekg(2 - (long) token.length(), std::ios_base::cur);
        return token.substr(0, 2);
    }

    // this should not be reached unless an illegal character is present
    return {};
}
}

