#include "calls_clause.h"

#include <utility>

#include "common/entity_type_enum.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable CallsClause::SynSyn(KnowledgeBase *pkb, Synonym *first,
                                Synonym *second) const noexcept {
    auto [col_1, col_2] = pkb->GetCallsPairs();
    return {first, std::move(col_1), second, std::move(col_2)};
}
ResultTable CallsClause::SynProc(KnowledgeBase *pkb, Synonym *first,
                                 ProcName second) const noexcept {
    auto result = pkb->GetCalls(Name<ArgPos::kSecond>(second));
    return {first, std::move(result)};
}
ResultTable CallsClause::ProcSyn(KnowledgeBase *pkb, ProcName first,
                                 Synonym *second) const noexcept {
    auto result = pkb->GetCalls(Name<ArgPos::kFirst>(first));
    return {second, std::move(result)};
}
ResultTable CallsClause::ProcProc(KnowledgeBase *pkb, ProcName first,
                                  ProcName second) const noexcept {
    return ResultTable(pkb->ExistCalls(first, second));
}
}  // namespace spa
