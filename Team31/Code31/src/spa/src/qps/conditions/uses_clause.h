#ifndef SRC_SPA_SRC_QPS_CONDITIONS_USES_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_USES_CLAUSE_H_

#include "uses_modifies_base.h"

namespace spa {
class UsesClause : public UsesModifiesBase {
    using UsesModifiesBase::UsesModifiesBase;
    ResultTable Execute(KnowledgeBase *knowledge_base) const final;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_USES_CLAUSE_H_
