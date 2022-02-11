#ifndef SPA_SRC_SPA_SRC_PKB_VARIABLE_NAME_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_VARIABLE_NAME_STORE_H_

#include <string>
#include <unordered_map>
#include <vector>

namespace spa {
class VariableNameStore {
  public:
    explicit VariableNameStore(std::vector<std::string> notations);
    std::string GetName(int index);
    int GetIndex(std::string name);

  private:
    std::vector<std::string> index_to_name;
    std::unordered_map<std::string, int> name_to_index;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_PKB_VARIABLE_NAME_STORE_H_
