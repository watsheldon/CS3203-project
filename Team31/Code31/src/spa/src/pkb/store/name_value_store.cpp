#include "name_value_store.h"

#include <cassert>
#include <utility>

namespace spa {
NameValueStore::NameValueStore(std::vector<std::string> &&proc_names,
                               std::vector<std::string> &&var_names,
                               std::vector<std::string> &&const_values) {
    name_val_store_.emplace_back(
            std::move(IndexBimap<std::string>(std::move(proc_names))));
    name_val_store_.emplace_back(
            std::move(IndexBimap<std::string>(std::move(var_names))));
    name_val_store_.emplace_back(
            std::move(IndexBimap<std::string>(std::move(const_values))));
}

const std::string &NameValueStore::GetNameValue(int index,
                                                QueryEntityType type) const {
    assert(type != QueryEntityType::kStmt);
    return name_val_store_[static_cast<int>(type)].GetVal(index);
}

int NameValueStore::GetIndex(const std::string &name,
                             QueryEntityType type) const {
    assert(type != QueryEntityType::kStmt);
    return name_val_store_[static_cast<int>(type)].GetKey(name);
}

const std::vector<std::string> &NameValueStore::GetAllNamesValues(
        QueryEntityType type) const {
    assert(type != QueryEntityType::kStmt);
    return name_val_store_[static_cast<int>(type)].GetAllVals();
}

size_t NameValueStore::GetSize(QueryEntityType type) const {
    assert(type != QueryEntityType::kStmt);
    return name_val_store_[static_cast<int>(type)].GetValsSize();
}
}  // namespace spa
