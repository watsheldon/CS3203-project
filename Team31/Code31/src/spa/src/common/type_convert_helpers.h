#ifndef SRC_SPA_SRC_COMMON_TYPE_CONVERT_HELPERS_H_
#define SRC_SPA_SRC_COMMON_TYPE_CONVERT_HELPERS_H_

#include <cassert>

#include "entity_type_enum.h"
#include "qps/synonym.h"

namespace spa {
constexpr StmtType SynToPkbType(const Synonym* synonym) noexcept {
    assert(synonym->type < Synonym::kProc);
    return static_cast<StmtType>(synonym->type);
}
constexpr QueryEntityType AttrToPkbType(const Attribute attribute) noexcept {
    return static_cast<QueryEntityType>(attribute);
}
}  // namespace spa

#endif  // SRC_SPA_SRC_COMMON_TYPE_CONVERT_HELPERS_H_
