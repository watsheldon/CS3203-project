#include "cache.h"

namespace spa {
Cache::Cache(size_t max_size) noexcept
        : cache_items_map_(max_size,
                           std::vector<Cache::Indicator>(
                                   max_size, Cache::Indicator::kUncalculated)) {
}
void Cache::Set(StmtNo stmt1, StmtNo stmt2, Cache::Indicator value) noexcept {
    cache_items_map_[stmt1][stmt2] = value;
}
Cache::Indicator Cache::Get(StmtNo stmt1, StmtNo stmt2) const noexcept {
    return cache_items_map_[stmt1][stmt2];
}
}  // namespace spa
