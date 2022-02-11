#include "procedure_name_store.h"
using namespace spa;
ProcedureNameStore::ProcedureNameStore(std::vector<std::string> names)
        : index_to_name(names) {
    for (int i = 0; i < names.size(); ++i) {
        name_to_index[names[i]] = i;
    }
}
std::string ProcedureNameStore::GetName(int index) {
    return index_to_name.at(index);
}
int ProcedureNameStore::GetIndex(std::string name) {
    if (name_to_index.find(name) == name_to_index.end()) {
        return -1;
    } else {
        return name_to_index.at(name);
    }
}
