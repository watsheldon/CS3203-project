#include "constant_value_store.h"

#include "knowledge_base.h"

namespace spa {
ConstantValueStore ::ConstantValueStore(std::vector<std::string>&& values)
        : index_to_value(values) {
    for (int i = 1; i < index_to_value.size(); ++i) {
        value_to_index[index_to_value.at(i)] = i;
    }
}
const std::string& ConstantValueStore ::GetValue(Index<kConst> index) const {
    return index_to_value.at(index.value);
}
Index<kConst> ConstantValueStore ::GetIndex(const std::string& value) const {
    if (value_to_index.find(value) == value_to_index.end()) {
        return Index<kConst>(0);
    }
    return Index<kConst>(value_to_index.at(value));
}
const std::vector<std::string>& ConstantValueStore::GetAllValue() const {
    return index_to_value;
}
}  // namespace spa