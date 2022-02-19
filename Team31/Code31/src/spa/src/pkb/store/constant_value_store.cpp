#include "constant_value_store.h"

#include "pkb/knowledge_base.h"

namespace spa {
ConstantValueStore ::ConstantValueStore(std::vector<std::string>&& values)
        : index_to_value_(std::move(values)) {
    for (int i = 1; i < index_to_value_.size(); ++i) {
        value_to_index_.emplace(index_to_value_[i], i);
    }
}
const std::string& ConstantValueStore ::GetValue(int index) const {
    return index_to_value_[index];
}
int ConstantValueStore ::GetIndex(const std::string& value) const {
    auto iter = value_to_index_.find(value);
    if (iter == value_to_index_.end()) {
        return 0;
    }
    return iter->second;
}
const std::vector<std::string>& ConstantValueStore::GetAllValues() const {
    return index_to_value_;
}
size_t ConstantValueStore::size() const { return index_to_value_.size() - 1; }
}  // namespace spa
