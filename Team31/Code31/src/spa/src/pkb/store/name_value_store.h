#ifndef SRC_SPA_SRC_PKB_STORE_NAME_VALUE_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_NAME_VALUE_STORE_H_

#include <map>
#include <string>
#include <vector>

#include "pkb/knowledge_base.h"

namespace spa {
class NameValueStore {
  public:
    explicit NameValueStore(std::vector<std::string> &&names);
    [[nodiscard]] const std::string &GetNameValue(int index) const;
    [[nodiscard]] int GetIndex(const std::string &name) const;
    [[nodiscard]] const std::vector<std::string> &GetAllNamesValues() const;
    [[nodiscard]] size_t size() const;

  private:
    std::vector<std::string> index_to_name_;
    std::map<std::string, int> name_to_index_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_NAME_VALUE_STORE_H_
