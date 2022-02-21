#ifndef SRC_SPA_SRC_COMMON_ALIASES_H_
#define SRC_SPA_SRC_COMMON_ALIASES_H_

#include <memory>
#include <vector>

namespace spa {
template <typename T>
using VecUniquePtr = std::vector<std::unique_ptr<T>>;
}

#endif  // SRC_SPA_SRC_COMMON_ALIASES_H_
