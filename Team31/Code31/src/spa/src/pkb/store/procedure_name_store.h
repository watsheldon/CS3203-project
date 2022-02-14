#ifndef SPA_SRC_SPA_SRC_PKB_STORE_PROCEDURE_NAME_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_STORE_PROCEDURE_NAME_STORE_H_

#include <map>
#include <string>
#include <vector>

#include "pkb/knowledge_base.h"

namespace spa {
class ProcedureNameStore {
  public:
    explicit ProcedureNameStore(std::vector<std::string> &&names);
    [[nodiscard]] const std::string &GetName(int index) const;
    [[nodiscard]] int GetIndex(const std::string &name) const;
    [[nodiscard]] const std::vector<std::string> &GetAllNames() const;
    [[nodiscard]] size_t size() const;

  private:
    std::vector<std::string> index_to_name_;
    std::map<std::string, int> name_to_index_;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_PKB_STORE_PROCEDURE_NAME_STORE_H_
