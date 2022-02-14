#ifndef SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_INDEX_H_
#define SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_INDEX_H_

#include <type_traits>

#include "common/entity_type_enum.h"

namespace spa {
template <typename EnumClass, EnumClass, EnumClass EnumVal>
struct IndexBase {
    static_assert(std::is_same_v<EnumClass, SetEntityType> ||
                  std::is_same_v<EnumClass, QueryEntityType> ||
                  std::is_same_v<EnumClass, GetPos>);
    explicit IndexBase(int i = 0) : value(i) {}
    const int value;
};
template <auto EnumVal>
struct Index : IndexBase<decltype(EnumVal), EnumVal> {
    using IndexBase<decltype(EnumVal), EnumVal>::IndexBase;
};

}  // namespace spa

#endif  // SRC_SPA_SRC_PKB_KNOWLEDGE_BASE_H_INDEX_H_
