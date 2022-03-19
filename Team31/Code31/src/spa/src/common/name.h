#ifndef SPA_SRC_SPA_SRC_COMMON_NAME_H_
#define SPA_SRC_SPA_SRC_COMMON_NAME_H_

#include <string_view>
#include <type_traits>

#include "entity_type_enum.h"

namespace spa {
template <typename EnumClass, EnumClass EnumVal>
struct NameBase {
    static_assert(std::is_same_v<EnumClass, ArgPos>);
    explicit NameBase(std::string_view name) noexcept : value(name) {}
    const std::string_view value;
};
template <auto EnumVal>
struct Name : NameBase<decltype(EnumVal), EnumVal> {
    using NameBase<decltype(EnumVal), EnumVal>::NameBase;
};
}  // namespace spa

#endif  // SPA_SRC_SPA_SRC_COMMON_NAME_H_
