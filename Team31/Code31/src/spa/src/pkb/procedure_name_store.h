#ifndef SPA_SRC_SPA_SRC_PKB_PROCEDURE_NAME_STORE_H_
#define SPA_SRC_SPA_SRC_PKB_PROCEDURE_NAME_STORE_H_

#include <map>
#include <string>
#include <vector>

#include "knowledge_base.h"

namespace spa {
class ProcedureNameStore {
  public:
    explicit ProcedureNameStore(std::vector<std::string> &&names);
    [[nodiscard]] const std::string &GetName(Index<kProc> index) const;
    [[nodiscard]] Index<kProc> GetIndex(const std::string &name) const;
    [[nodiscard]] const std::vector<std::string> &GetAllName() const;

  private:
    std::vector<std::string> index_to_name;
    std::map<std::string, int> name_to_index;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_PKB_PROCEDURE_NAME_STORE_H_
