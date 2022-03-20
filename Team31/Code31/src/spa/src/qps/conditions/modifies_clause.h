#ifndef SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_

#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"
#include "uses_modifies_base.h"

namespace spa {
class ModifiesClause : public UsesModifiesBase {
    using UsesModifiesBase::UsesModifiesBase;
    ResultTable Execute(KnowledgeBase *knowledge_base) const final;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_MODIFIES_CLAUSE_H_
