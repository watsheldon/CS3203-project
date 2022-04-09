#include "calls_trans_clause.h"

#include <utility>

#include "common/entity_type_enum.h"
#include "common/type_convert_helpers.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable CallsTransClause::SynSyn(KnowledgeBase *pkb, Synonym *first,
                                     Synonym *second) const noexcept {
    auto [col_1, col_2] = pkb->GetCallsTPairs();
    return {first, std::move(col_1), second, std::move(col_2)};
}
ResultTable CallsTransClause::SynProc(KnowledgeBase *pkb, Synonym *first,
                                      ProcName second) const noexcept {
    auto result = pkb->GetCallsT(Name<ArgPos::kSecond>(second));
    return {first, std::move(result)};
}
ResultTable CallsTransClause::ProcSyn(KnowledgeBase *pkb, ProcName first,
                                      Synonym *second) const noexcept {
    auto result = pkb->GetCallsT(Name<ArgPos::kFirst>(first));
    return {second, std::move(result)};
}
ResultTable CallsTransClause::ProcProc(KnowledgeBase *pkb, ProcName first,
                                       ProcName second) const noexcept {
    return ResultTable(pkb->ExistCallsT(first, second));
}
int CallsTransClause::GetPriority() const noexcept { return kPriority; }
}  // namespace spa
