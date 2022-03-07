#ifndef SRC_SPA_SRC_PKB_STORE_NAME_VALUE_STORE_H_
#define SRC_SPA_SRC_PKB_STORE_NAME_VALUE_STORE_H_

#include <string>

#include "common/entity_type_enum.h"
#include "index_bimap.h"

namespace spa {
// Stores the index to name/value mappings for procedure, var and constant.
class NameValueStore {
  public:
    explicit NameValueStore(std::vector<std::string> &&proc_names,
                            std::vector<std::string> &&var_names,
                            std::vector<std::string> &&const_values);
    [[nodiscard]] const std::string &GetNameValue(int index,
                                                  QueryEntityType type) const;
    [[nodiscard]] int GetIndex(const std::string &name,
                               QueryEntityType type) const;
    [[nodiscard]] const std::vector<std::string> &GetAllNamesValues(
            QueryEntityType type) const;
    [[nodiscard]] size_t GetSize(QueryEntityType type) const;

  private:
    /**
     * A vector of bimaps. Vector is of size 3 and each element in order is
     * proc_bimap, var_bimap, const_bimap.
     */
    std::vector<IndexBimap<std::string>> name_val_store_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_STORE_NAME_VALUE_STORE_H_
