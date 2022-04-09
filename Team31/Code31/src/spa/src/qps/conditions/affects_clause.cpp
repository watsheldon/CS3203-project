#include "affects_clause.h"

#include <utility>

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/type_convert_helpers.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"
#include "qps/synonym.h"

namespace spa {
ResultTable AffectsClause::NumNum(KnowledgeBase *pkb, StmtNo first,
                                  StmtNo second) const noexcept {
    auto result = pkb->ExistAffects(Index<ArgPos::kFirst>(first),
                                    Index<ArgPos::kSecond>(second));
    return ResultTable(result);
}
ResultTable AffectsClause::NumSyn(KnowledgeBase *pkb, StmtNo first,
                                  Synonym *second) const noexcept {
    auto result = pkb->GetAffects(Index<ArgPos::kFirst>(first));
    return {second, std::move(result)};
}
ResultTable AffectsClause::SynNum(KnowledgeBase *pkb, Synonym *first,
                                  StmtNo second) const noexcept {
    auto result = pkb->GetAffects(Index<ArgPos::kSecond>(second));
    return {first, std::move(result)};
}
ResultTable AffectsClause::SynSyn(KnowledgeBase *pkb, Synonym *first,
                                  Synonym *second) const noexcept {
    if (first == second) {
        auto result = pkb->GetAffectsSelf();
        return {first, std::move(result)};
    }
    auto [col_1, col_2] = pkb->GetAffectsPairs();
    return {first, std::move(col_1), second, std::move(col_2)};
}
int AffectsClause::GetPriority() const noexcept { return kPriority; }
}  // namespace spa
