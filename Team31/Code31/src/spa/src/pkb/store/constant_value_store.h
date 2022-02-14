#ifndef SPA_SRC_SPA_SRC_PKB_STORE_CONSTANT_VALUE_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_STORE_CONSTANT_VALUE_STORE_H_

#include <map>
#include <string>
#include <vector>

#include "pkb/knowledge_base.h"

namespace spa {
class ConstantValueStore {
  public:
    explicit ConstantValueStore(std::vector<std::string> &&values);
    [[nodiscard]] const std::string &GetValue(int index) const;
    [[nodiscard]] int GetIndex(const std::string &value) const;
    [[nodiscard]] const std::vector<std::string> &GetAllValues() const;
    [[nodiscard]] size_t size() const;

  private:
    std::vector<std::string> index_to_value_;
    std::map<std::string, int> value_to_index_;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_PKB_STORE_CONSTANT_VALUE_STORE_H_
