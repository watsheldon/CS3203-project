#ifndef SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CACHE_H_
#define SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CACHE_H_

#include <vector>

#include "common/aliases.h"

namespace spa {
class Cache {
  public:
    enum class Indicator {
        kUncalculated = 0,
        kCalculatedTrue,
        kCalculatedFalse
    };
    Cache(size_t max_size);
    void Put(const StmtNo& stmt1, const StmtNo& stmt2,
             const Cache::Indicator& value);
    const Cache::Indicator& Get(const StmtNo& stmt1, const StmtNo& stmt2) const;
    size_t Size() const noexcept;

  private:
    std::vector<std::vector<Cache::Indicator>> cache_items_map_;
    size_t max_cache_size_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CACHE_H_
