#ifndef SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CACHE_H_
#define SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CACHE_H_

#include <vector>

#include "common/aliases.h"

namespace spa {
class Cache {
  public:
    enum class Indicator { kUncalculated = 0, kTrue, kFalse };
    explicit Cache(size_t max_size) noexcept;
    void Set(StmtNo stmt1, StmtNo stmt2, Indicator value) noexcept;
    void Clear() noexcept;
    [[nodiscard]] Indicator Get(StmtNo stmt1, StmtNo stmt2) const noexcept;

  private:
    Vec2D<Indicator> cache_items_map_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CACHE_H_
