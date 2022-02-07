#include "validator.h"

#include <memory>

namespace spa {
Validator::Validator(const std::filesystem::path &filepath) : tokenizer_(
        filepath) {

}
std::shared_ptr<std::vector<Token>> spa::Validator::Validate() {
    return {};
}
}
