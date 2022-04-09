#include "cache.h"

namespace spa {
Cache::Cache(size_t max_size)
        : max_cache_size_(max_size),
          cache_items_map_(max_size,
                           std::vector<Cache::Indicator>(
                                   max_size, Cache::Indicator::kUncalculated)) {
}
void Cache::Put(const StmtNo& stmt1, const StmtNo& stmt2,
                const Cache::Indicator& value) {
    cache_items_map_[stmt1][stmt2] = value;
}
const Cache::Indicator& Cache::Get(const StmtNo& stmt1,
                                   const StmtNo& stmt2) const {
    return cache_items_map_[stmt1][stmt2];
}
size_t Cache::Size() const noexcept { return cache_items_map_.size(); }
}  // namespace spa
