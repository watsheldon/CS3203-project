#ifndef SPA_SRC_SPA_SRC_PKB_CONSTANT_VALUE_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_CONSTANT_VALUE_STORE_H_

#include <map>
#include <string>
#include <vector>

#include "knowledge_base.h"

namespace spa {
class ConstantValueStore {
  public:
    explicit ConstantValueStore(std::vector<std::string> &&values);
    [[nodiscard]] const std::string &GetValue(Index<kConst> index) const;
    [[nodiscard]] Index<kConst> GetIndex(const std::string &value) const;
    [[nodiscard]] const std::vector<std::string> &GetAllValue() const;

  private:
    std::vector<std::string> index_to_value;
    std::map<std::string, int> value_to_index;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_PKB_CONSTANT_VALUE_STORE_H_
