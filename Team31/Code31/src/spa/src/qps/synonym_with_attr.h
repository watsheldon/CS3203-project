#ifndef SRC_SPA_SRC_QPS_SYNONYM_WITH_ATTR_H_
#define SRC_SPA_SRC_QPS_SYNONYM_WITH_ATTR_H_

#include "common/entity_type_enum.h"
#include "synonym.h"

namespace spa {
struct SynonymWithAttr {
    explicit SynonymWithAttr(Synonym* synonym,
                             Attribute attribute = Attribute::kNone)
            : synonym_(synonym), attribute_(attribute){};

    Synonym* const synonym_;
    const Attribute attribute_;
    bool operator<(const SynonymWithAttr& other) const noexcept {
        return synonym_ < other.synonym_;
    }
    bool operator==(const SynonymWithAttr& other) const noexcept {
        return false;
    }
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_SYNONYM_WITH_ATTR_H_
