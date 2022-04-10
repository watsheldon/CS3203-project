#include "cache.h"

namespace spa {
Cache::Cache(size_t max_size) noexcept
        : cache_items_map_(
                  max_size,
                  std::vector<Indicator>(max_size, Indicator::kUncalculated)) {}
void Cache::Set(StmtNo stmt1, StmtNo stmt2, Indicator value) noexcept {
    cache_items_map_[stmt1][stmt2] = value;
}
Cache::Indicator Cache::Get(StmtNo stmt1, StmtNo stmt2) const noexcept {
    return cache_items_map_[stmt1][stmt2];
}
void Cache::Clear() noexcept {
    size_t size = cache_items_map_.size();
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            cache_items_map_[i][j] = Indicator::kUncalculated;
        }
    }
}
}  // namespace spa
