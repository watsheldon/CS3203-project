#include "procedure_name_store.h"
namespace spa {
ProcedureNameStore::ProcedureNameStore(std::vector<std::string>&& names)
        : index_to_name(names) {
    for (int i = 1; i < index_to_name.size(); ++i) {
        name_to_index[index_to_name.at(i)] = i;
    }
}
const std::string& ProcedureNameStore::GetName(Index<kProc> index) const {
    return index_to_name.at(index.value);
}
Index<kProc> ProcedureNameStore::GetIndex(const std::string& name) const {
    if (name_to_index.find(name) == name_to_index.end()) {
        return Index<kProc>(0);
    }
    return Index<kProc>(name_to_index.at(name));
}
const std::vector<std::string>& ProcedureNameStore::GetAllName() const {
    return index_to_name;
}
}  // namespace spa
