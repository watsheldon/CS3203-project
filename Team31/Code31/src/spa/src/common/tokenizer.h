#ifndef SRC_SPA_SRC_COMMON_TOKENIZER_H_
#define SRC_SPA_SRC_COMMON_TOKENIZER_H_

#include <filesystem>
#include <sstream>
#include <string_view>

namespace spa {
/**
 * Tokenizer class that extracts a single token each time using simple rules.
 */
class Tokenizer {
  public:
    /**
     * Constructs a new Tokenizer that reads in the file located at `filepath`.
     */
    explicit Tokenizer(const std::filesystem::path &filepath);
    /**
     * Constructs a new Tokenizer with `str` as the initial content.
     * @param str The initial content of the Tokenizer
     */
    explicit Tokenizer(std::string_view str);
    /**
     * Updates the internal content of the Tokenizer and clears any error.
     * @param str The new content of the Tokenizer
     * @return The current instance of the Tokenizer
     */
    Tokenizer &operator()(std::string_view str);
    Tokenizer(Tokenizer &&tokenizer) = delete;

    /**
     * Retrieves the next token if available.
     * @return the next valid token if success, empty string otherwise
     */
    std::string Next();
    std::string Peek();

    /**
     * The stream operator to retrieve the next token.
     * @param token to store the extracted token.
     */
    Tokenizer &operator>>(std::string &token);

  private:
    static constexpr char kSpecialSingle[] = "%()*+-/;{}";
    static constexpr char kSpecialMaybePaired[] = "!<=>";
    static constexpr char kSpecialPaired[] = "&|";
    std::stringstream buffer_;
    bool error = false;  // triggered by either EOF or unrecognized token
    std::string peek_;

    void KeepWhile(std::string &token, int (*pred)(int));
    void KeepFirstOf(std::string &token, long len);
    void ExtractInto(std::string &token);
};
}  // namespace spa

#endif  // SRC_SPA_SRC_COMMON_TOKENIZER_H_
