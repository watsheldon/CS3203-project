#include "variable_name_store.h"
namespace spa {
VariableNameStore::VariableNameStore(std::vector<std::string>&& names)
        : index_to_name(names) {
    for (int i = 1; i < names.size(); ++i) {
        name_to_index[index_to_name.at(i)] = i;
    }
}

const std::string& VariableNameStore::GetName(Index<kVar> index) const {
    return index_to_name.at(index.value);
}
Index<kVar> VariableNameStore::GetIndex(const std::string& name) const {
    if (name_to_index.find(name) == name_to_index.end()) {
        return Index<kVar>(0);
    }
    return Index<kVar>(name_to_index.at(name));
}
const std::vector<std::string>& VariableNameStore::GetAllName() const {
    return index_to_name;
}
}  // namespace spa
