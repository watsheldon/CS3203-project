#ifndef SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CACHE_H_
#define SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CACHE_H_

#include <deque>
#include <iterator>
#include <mutex>
#include <set>
#include <unordered_map>
#include <utility>

namespace spa {
class Cache {
  public:
    Cache(size_t max_size);
    void Put(const int& key, const int& value);
    const std::set<int>& Get(const int& key) const;
    bool Exists(const int& key) const noexcept;
    size_t Size() const noexcept;

  private:
    std::unordered_map<int, std::set<int>> cache_items_map;
    size_t max_cache_size;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_SECONDARY_STRUCTURE_CACHE_H_
