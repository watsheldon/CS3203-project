#include "constant_value_store.h"

#include "knowledge_base.h"

namespace spa {
ConstantValueStore ::ConstantValueStore(std::vector<std::string>&& values)
        : index_to_value_(values) {
    for (int i = 1; i < index_to_value_.size(); ++i) {
        value_to_index_.emplace(index_to_value_.at(i), i);
    }
}
const std::string& ConstantValueStore ::GetValue(int index) const {
    return index_to_value_.at(index);
}
int ConstantValueStore ::GetIndex(const std::string& value) const {
    if (value_to_index_.find(value) == value_to_index_.end()) {
        return 0;
    }
    return value_to_index_.at(value);
}
const std::vector<std::string>& ConstantValueStore::GetAllValue() const {
    return index_to_value_;
}
size_t ConstantValueStore::GetCount() const {
    return index_to_value_.size() - 1;
}
}  // namespace spa
