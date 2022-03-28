#ifndef SRC_SPA_SRC_COMMON_VARIANT_OVERLOAD_H_
#define SRC_SPA_SRC_COMMON_VARIANT_OVERLOAD_H_

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

#endif  // SRC_SPA_SRC_COMMON_VARIANT_OVERLOAD_H_
