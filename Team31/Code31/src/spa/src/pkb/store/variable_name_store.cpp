#include "variable_name_store.h"
namespace spa {
VariableNameStore::VariableNameStore(std::vector<std::string>&& names)
        : index_to_name_(std::move(names)) {
    for (int i = 1; i < index_to_name_.size(); ++i) {
        name_to_index_.emplace(index_to_name_[i], i);
    }
}

const std::string& VariableNameStore::GetName(int index) const {
    return index_to_name_[index];
}
int VariableNameStore::GetIndex(const std::string& name) const {
    auto iter = name_to_index_.find(name);
    if (iter == name_to_index_.end()) {
        return 0;
    }
    return iter->second;
}
const std::vector<std::string>& VariableNameStore::GetAllNames() const {
    return index_to_name_;
}
size_t VariableNameStore::size() const { return index_to_name_.size() - 1; }
}  // namespace spa