#ifndef SRC_SPA_SRC_COMMON_ALIASES_H_
#define SRC_SPA_SRC_COMMON_ALIASES_H_

#include <memory>
#include <string_view>
#include <vector>

namespace spa {
template <typename T>
using VecUniquePtr = std::vector<std::unique_ptr<T>>;
template <typename T>
using PairOf = std::pair<T, T>;
template <typename T, typename U = T>
using PairVec = std::pair<std::vector<T>, std::vector<U>>;
template <typename T>
using Vec2D = std::vector<std::vector<T>>;
template <typename T, size_t row, size_t col>
using Array2D = std::array<std::array<T, col>, row>;
using StmtNo = int;
using StmtLstIndex = int;
using VarIndex = int;
using ProcIndex = int;
using Ident = std::string;
using IdentView = std::string_view;
}  // namespace spa

#endif  // SRC_SPA_SRC_COMMON_ALIASES_H_
