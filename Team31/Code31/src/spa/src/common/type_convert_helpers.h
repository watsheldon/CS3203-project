#ifndef SRC_SPA_SRC_COMMON_TYPE_CONVERT_HELPERS_H_
#define SRC_SPA_SRC_COMMON_TYPE_CONVERT_HELPERS_H_

#include <cassert>

#include "entity_type_enum.h"
#include "qps/synonym.h"

namespace spa {
constexpr StmtType SynToPkbType(const Synonym* synonym) {
    switch (synonym->type) {
        case Synonym::kStmtAny:
            return StmtType::kAll;
        case Synonym::kStmtRead:
            return StmtType::kRead;
        case Synonym::kStmtPrint:
            return StmtType::kPrint;
        case Synonym::kStmtCall:
            return StmtType::kCall;
        case Synonym::kStmtWhile:
            return StmtType::kWhile;
        case Synonym::kStmtIf:
            return StmtType::kIf;
        case Synonym::kStmtAssign:
            return StmtType::kAssign;
        default:
            assert(false);
    }
}
}  // namespace spa

#endif  // SRC_SPA_SRC_COMMON_TYPE_CONVERT_HELPERS_H_
