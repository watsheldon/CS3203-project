#include "uses_clause.h"

#include <utility>

#include "common/index.h"
#include "common/type_convert_helpers.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable UsesClause::NumVar(KnowledgeBase *pkb, StmtNo first,
                               VarName second) const noexcept {
    auto result = pkb->ExistUses(first, second);
    return ResultTable(result);
}
ResultTable UsesClause::NumSyn(KnowledgeBase *pkb, StmtNo first,
                               Synonym *second) const noexcept {
    auto result = pkb->GetUses(Index<QueryEntityType::kStmt>(first));
    return {second, std::move(result)};
}
ResultTable UsesClause::NumWild(KnowledgeBase *pkb,
                                StmtNo first) const noexcept {
    auto result = pkb->ExistUses(first, KnowledgeBase::kWildCard);
    return ResultTable(result);
}
ResultTable UsesClause::SynVar(KnowledgeBase *pkb, Synonym *first,
                               VarName second) const noexcept {
    auto result = first->type == Synonym::kProc
                          ? pkb->GetUses(Name<ArgPos::kSecond>(second))
                          : pkb->GetUses(second, SynToPkbType(first));
    return {first, std::move(result)};
}
ResultTable UsesClause::SynSyn(KnowledgeBase *pkb, Synonym *first,
                               Synonym *second) const noexcept {
    auto [col_1, col_2] = first->type == Synonym::kProc
                                  ? pkb->GetUsesProcVar()
                                  : pkb->GetUsesStmtVar(SynToPkbType(first));
    return {first, std::move(col_1), second, std::move(col_2)};
}
ResultTable UsesClause::SynWild(KnowledgeBase *pkb,
                                Synonym *first) const noexcept {
    auto result = first->type == Synonym::kProc
                          ? pkb->GetUsesProc()
                          : pkb->GetUses(SynToPkbType(first));
    return {first, std::move(result)};
}
ResultTable UsesClause::ProcVar(KnowledgeBase *pkb, ProcName first,
                                VarName second) const noexcept {
    auto result = pkb->ExistUses(first, second);
    return ResultTable(result);
}
ResultTable UsesClause::ProcSyn(KnowledgeBase *pkb, ProcName first,
                                Synonym *second) const noexcept {
    auto result = pkb->GetUses(Name<ArgPos::kFirst>(first));
    return {second, std::move(result)};
}
ResultTable UsesClause::ProcWild(KnowledgeBase *pkb,
                                 ProcName first) const noexcept {
    return ResultTable(pkb->ExistUses(first));
}
int UsesClause::GetPriority() const noexcept { return kPriority; }
}  // namespace spa
