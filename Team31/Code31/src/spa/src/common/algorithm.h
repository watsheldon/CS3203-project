#ifndef SRC_SPA_SRC_COMMON_ALGORITHM_H_
#define SRC_SPA_SRC_COMMON_ALGORITHM_H_

namespace spa {
template <class InputIt, class T>
constexpr InputIt find(InputIt first, InputIt last, const T& value) {
    while (first != last && *first != value) {
        ++first;
    }
    return first;
}
}  // namespace spa

#endif  // SRC_SPA_SRC_COMMON_ALGORITHM_H_
