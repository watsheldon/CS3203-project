#ifndef SRC_SPA_SRC_QPS_PQL_TOKENIZER_H_
#define SRC_SPA_SRC_QPS_PQL_TOKENIZER_H_

#include <filesystem>
#include <sstream>
#include <string_view>

namespace spa {
class PQLTokenizer {
  private:
    static constexpr char kSpecialSingle[] = "%()*+-/;,_";
    std::stringstream buffer_;
    bool error = false;  // triggered by either EOF or unrecognized token

    void KeepWhile(std::string &token, int (*pred)(int));
    void KeepFirstOf(std::string &token, long len);
    void ExtractInto(std::string &token);

  public:
    explicit PQLTokenizer(const std::filesystem::path &inputFile);
    explicit PQLTokenizer(std::string_view str);
    PQLTokenizer &operator()(std::string_view str);
    PQLTokenizer &operator>>(std::string &token);
    std::string Next();
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_PQL_TOKENIZER_H_
