#ifndef SRC_SPA_SRC_SOURCE_TOKENIZER_H_
#define SRC_SPA_SRC_SOURCE_TOKENIZER_H_

#include <string_view>
#include <sstream>
#include <filesystem>

namespace spa {
class Tokenizer {
  public:
    explicit Tokenizer(const std::filesystem::path &filepath);
    Tokenizer(Tokenizer &&tokenizer) = delete;

    /**
     * Retrieves the Next token if available.
     * @return the Next valid token if success, empty string otherwise
     */
    std::string Next();

  private:
    static constexpr char kSpecialSingle[] = "%()*+-/;{}";
    static constexpr char kSpecialMaybePaired[] = "!<=>";
    static constexpr char kSpecialPaired[] = "&|";
    std::stringstream buffer_;
};
}

#endif //SRC_SPA_SRC_SOURCE_TOKENIZER_H_
