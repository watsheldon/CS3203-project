#include "uses_clause.h"

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/type_convert_helpers.h"

namespace spa {
UsesIntIdent::UsesIntIdent(int first, std::string second)
        : first_(first), second_(std::move(second)) {}
ResultTable UsesIntIdent::Execute(KnowledgeBase *knowledge_base) const {
    auto index_2nd =
            knowledge_base->NameToIndex(QueryEntityType::kVar, second_);
    if (index_2nd == 0) return ResultTable(false);
    auto result = knowledge_base->ExistUses(first_, index_2nd);
    return ResultTable(result);
}
UsesIntSyn::UsesIntSyn(int first, Synonym *second)
        : first_(first), second_(second) {}
ResultTable UsesIntSyn::Execute(KnowledgeBase *knowledge_base) const {
    auto result =
            knowledge_base->GetUses(Index<QueryEntityType::kStmt>(first_));
    return {second_, std::move(result)};
}
UsesIntWild::UsesIntWild(int first) : first_(first) {}
ResultTable UsesIntWild::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->ExistUses(first_, 0);
    return ResultTable(result);
}
UsesSynIdent::UsesSynIdent(Synonym *first, std::string second)
        : first_(first), second_(std::move(second)) {}
ResultTable UsesSynIdent::Execute(KnowledgeBase *knowledge_base) const {
    auto index_2nd =
            knowledge_base->NameToIndex(QueryEntityType::kVar, second_);
    if (index_2nd == 0) return ResultTable(false);
    auto result = knowledge_base->GetUses(
            Index<QueryEntityType::kVar>(index_2nd), SynToPkbType(first_));
    return {first_, std::move(result)};
}
UsesSynSyn::UsesSynSyn(Synonym *first, Synonym *second)
        : first_(first), second_(second) {}
ResultTable UsesSynSyn::Execute(KnowledgeBase *knowledge_base) const {
    auto [col_1, col_2] = knowledge_base->GetUsesStmtVar(SynToPkbType(first_));
    return {first_, std::move(col_1), second_, std::move(col_2)};
}
UsesSynWild::UsesSynWild(Synonym *first) : first_(first) {}
ResultTable UsesSynWild::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->GetUses(SynToPkbType(first_));
    return {first_, std::move(result)};
}
}  // namespace spa
