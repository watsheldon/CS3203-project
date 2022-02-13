#include "procedure_name_store.h"
namespace spa {
ProcedureNameStore::ProcedureNameStore(std::vector<std::string>&& names)
        : index_to_name_(names) {
    for (int i = 1; i < index_to_name_.size(); ++i) {
        name_to_index_.emplace(index_to_name_.at(i), i);
    }
}
const std::string& ProcedureNameStore::GetName(int index) const {
    return index_to_name_.at(index);
}
int ProcedureNameStore::GetIndex(const std::string& name) const {
    if (name_to_index_.find(name) == name_to_index_.end()) {
        return 0;
    }
    return name_to_index_.at(name);
}
const std::vector<std::string>& ProcedureNameStore::GetAllName() const {
    return index_to_name_;
}
size_t ProcedureNameStore::GetCount() const {
    return index_to_name_.size() - 1;
}
}  // namespace spa
