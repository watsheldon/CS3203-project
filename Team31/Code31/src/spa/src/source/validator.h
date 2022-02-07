#ifndef SRC_SPA_SRC_SOURCE_VALIDATOR_H_
#define SRC_SPA_SRC_SOURCE_VALIDATOR_H_

#include "tokenizer.h"

#include <filesystem>
#include <memory>
#include <vector>

#include "token.h"

namespace spa {
class Validator {
  public:
    explicit Validator(const std::filesystem::path &filepath);
    std::shared_ptr<std::vector<Token>> Validate();

  private:
    Tokenizer tokenizer_;
};
}

#endif //SRC_SPA_SRC_SOURCE_VALIDATOR_H_
