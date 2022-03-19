#ifndef SPA_SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_
#define SPA_SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_

#include "uses_modifies_base.h"

namespace spa {
class ModifiesClause : public UsesModifiesBase {
    using UsesModifiesBase::UsesModifiesBase;
    ResultTable Execute(KnowledgeBase *knowledge_base) const override;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_
