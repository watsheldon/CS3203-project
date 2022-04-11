#ifndef SRC_SPA_SRC_QPS_SYNONYM_WITH_ATTR_H_
#define SRC_SPA_SRC_QPS_SYNONYM_WITH_ATTR_H_

#include "common/entity_type_enum.h"
#include "synonym.h"

namespace spa {
struct SynonymWithAttr {
    explicit SynonymWithAttr(Synonym* synonym,
                             Attribute attribute = Attribute::kNone)
            : synonym(synonym), attribute(attribute){};

    Synonym* const synonym;
    const Attribute attribute;
    bool operator==(const SynonymWithAttr& other) const noexcept {
        return false;
    }
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_SYNONYM_WITH_ATTR_H_
