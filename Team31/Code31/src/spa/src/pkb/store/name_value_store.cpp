#include "name_value_store.h"

namespace spa {
NameValueStore::NameValueStore(std::vector<std::string>&& names)
        : IndexBimap<std::string>(
                  std::forward<std::vector<std::string>>(names)) {}

const std::string& NameValueStore::GetNameValue(int index) const {
    return IndexBimap<std::string>::GetVal(index);
}
int NameValueStore::GetIndex(const std::string& name) const {
    return IndexBimap<std::string>::GetKey(name);
}
const std::vector<std::string>& NameValueStore::GetAllNamesValues() const {
    return IndexBimap<std::string>::GetAllVals();
}
size_t NameValueStore::size() const {
    return IndexBimap<std::string>::GetValsSize();
}
}  // namespace spa
