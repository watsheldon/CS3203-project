#include "modifies_clause.h"

#include <utility>

#include "common/index.h"
#include "common/type_convert_helpers.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"

namespace spa {
ResultTable ModifiesClause::NumVar(KnowledgeBase *pkb, StmtNo first,
                                   VarName second) const noexcept {
    auto result = pkb->ExistModifies(first, second);
    return ResultTable(result);
}
ResultTable ModifiesClause::NumSyn(KnowledgeBase *pkb, StmtNo first,
                                   Synonym *second) const noexcept {
    auto result = pkb->GetModifies(Index<QueryEntityType::kStmt>(first));
    return {second, std::move(result)};
}
ResultTable ModifiesClause::NumWild(KnowledgeBase *pkb,
                                    StmtNo first) const noexcept {
    auto result = pkb->ExistModifies(first, KnowledgeBase::kWildCard);
    return ResultTable(result);
}
ResultTable ModifiesClause::SynVar(KnowledgeBase *pkb, Synonym *first,
                                   VarName second) const noexcept {
    auto result = first->type == Synonym::kProc
                          ? pkb->GetModifies(Name<ArgPos::kSecond>(second))
                          : pkb->GetModifies(second, SynToPkbType(first));
    return {first, std::move(result)};
}
ResultTable ModifiesClause::SynSyn(KnowledgeBase *pkb, Synonym *first,
                                   Synonym *second) const noexcept {
    auto [col_1, col_2] =
            first->type == Synonym::kProc
                    ? pkb->GetModifiesProcVar()
                    : pkb->GetModifiesStmtVar(SynToPkbType(first));
    return {first, std::move(col_1), second, std::move(col_2)};
}
ResultTable ModifiesClause::SynWild(KnowledgeBase *pkb,
                                    Synonym *first) const noexcept {
    auto result = first->type == Synonym::kProc
                          ? pkb->GetModifiesProc()
                          : pkb->GetModifies(SynToPkbType(first));
    return {first, std::move(result)};
}
ResultTable ModifiesClause::ProcVar(KnowledgeBase *pkb, ProcName first,
                                    VarName second) const noexcept {
    auto result = pkb->ExistModifies(first, second);
    return ResultTable(result);
}
ResultTable ModifiesClause::ProcSyn(KnowledgeBase *pkb, ProcName first,
                                    Synonym *second) const noexcept {
    auto result = pkb->GetModifies(Name<ArgPos::kFirst>(first));
    return {second, std::move(result)};
}
ResultTable ModifiesClause::ProcWild(KnowledgeBase *pkb,
                                     ProcName first) const noexcept {
    return ResultTable(pkb->ExistModifies(first));
}
}  // namespace spa
