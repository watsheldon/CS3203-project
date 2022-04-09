#include "cache.h"

namespace spa {
Cache::Cache(size_t max_size) : max_cache_size{max_size} {}
void Cache::Put(const int& key, const int& value) {
    auto it = cache_items_map.find(key);
    if (it == cache_items_map.end()) {
        cache_items_map.insert({key, {value}});
    } else {
        cache_items_map.at(key).insert(value);
    }
}
const std::set<int>& Cache::Get(const int& key) const {
    auto it = cache_items_map.find(key);
    if (it == cache_items_map.end()) {
        throw std::range_error("No such key in the cache");
    }
    return it->second;
}
bool Cache::Exists(const int& key) const noexcept {
    return cache_items_map.find(key) != cache_items_map.end();
}
size_t Cache::Size() const noexcept { return cache_items_map.size(); }
}  // namespace spa
