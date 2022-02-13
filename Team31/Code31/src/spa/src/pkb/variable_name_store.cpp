#include "variable_name_store.h"
namespace spa {
VariableNameStore::VariableNameStore(std::vector<std::string>&& names)
        : index_to_name_(names) {
    for (int i = 1; i < names.size(); ++i) {
        name_to_index_.emplace(index_to_name_.at(i), i);
    }
}

const std::string& VariableNameStore::GetName(int index) const {
    return index_to_name_.at(index);
}
int VariableNameStore::GetIndex(const std::string& name) const {
    if (name_to_index_.find(name) == name_to_index_.end()) {
        return 0;
    }
    return name_to_index_.at(name);
}
const std::vector<std::string>& VariableNameStore::GetAllName() const {
    return index_to_name_;
}
size_t VariableNameStore::GetCount() const { return index_to_name_.size() - 1; }
}  // namespace spa
