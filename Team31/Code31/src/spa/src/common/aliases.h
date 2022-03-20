#ifndef SRC_SPA_SRC_COMMON_ALIASES_H_
#define SRC_SPA_SRC_COMMON_ALIASES_H_

#include <memory>
#include <vector>

namespace spa {
template <typename T>
using VecUniquePtr = std::vector<std::unique_ptr<T>>;
template <typename T>
using Pair = std::pair<T, T>;
template <typename T>
using PairVec = Pair<std::vector<T>>;
template <typename T>
using Vec2D = std::vector<std::vector<T>>;
template <typename T, size_t row, size_t col>
using Array2D = std::array<std::array<T, col>, row>;
}  // namespace spa

#endif  // SRC_SPA_SRC_COMMON_ALIASES_H_
