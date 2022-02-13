#ifndef SRC_SPA_SRC_COMMON_LAZY_FACTORY_H_
#define SRC_SPA_SRC_COMMON_LAZY_FACTORY_H_

#include <algorithm>
#include <type_traits>

namespace spa {
/**
 * Adapted from
 * https://jguegant.github.io/blogs/tech/performing-try-emplace.html
 * @tparam Factory
 */
template <typename Factory>
struct LazyFactory {
    using result_type = std::invoke_result_t<const Factory &>;
    Factory factory_;

    constexpr explicit LazyFactory(Factory &&factory)
            : factory_(std::move(factory)) {}
    constexpr explicit operator result_type() const
            noexcept(std::is_nothrow_invocable_v<const Factory &>) {
        return factory_();
    }
};
}  // namespace spa

#endif  // SRC_SPA_SRC_COMMON_LAZY_FACTORY_H_
