#ifndef SPA_SRC_SPA_SRC_COMMON_MAP_H_
#define SPA_SRC_SPA_SRC_COMMON_MAP_H_

#include <array>
#include <cstddef>
#include <utility>

namespace spa {
template <typename Key, typename Value, std::size_t Size>
class Map {
  public:
    constexpr explicit Map(
            const std::array<std::pair<Key, Value>, Size> &array) noexcept
            : data_(array) {}
    constexpr Value at(const Key &key) const noexcept {
        for (const auto &[k, v] : data_) {
            if (k == key) {
                return v;
            }
        }
        return {};
    }

  private:
    const std::array<std::pair<Key, Value>, Size> data_;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_COMMON_MAP_H_
