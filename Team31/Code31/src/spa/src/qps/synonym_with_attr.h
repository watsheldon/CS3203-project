#ifndef SRC_SPA_SRC_QPS_SYNONYM_WITH_ATTR_H_
#define SRC_SPA_SRC_QPS_SYNONYM_WITH_ATTR_H_

#include "synonym.h"

namespace spa {
struct SynonymWithAttr {
    enum Attribute {
        kNone,
        kProcName,
        kVarName,
        kValue,
        kStmtNum,
    };
    explicit SynonymWithAttr(const Synonym* synonym,
                             Attribute attribute = kNone)
            : synonym(synonym), attribute(attribute){};
    const Synonym* const synonym;
    const Attribute attribute;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_SYNONYM_WITH_ATTR_H_
