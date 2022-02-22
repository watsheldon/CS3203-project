#include "modifies_clause.h"

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/type_convert_helpers.h"

namespace spa {
ModifiesIntIdent::ModifiesIntIdent(int first, std::string second)
        : first_(first), second_(std::move(second)) {}
ResultTable ModifiesIntIdent::Execute(KnowledgeBase *knowledge_base) const {
    auto index_2nd =
            knowledge_base->NameToIndex(QueryEntityType::kVar, second_);
    if (index_2nd == 0) return ResultTable(false);
    auto result = knowledge_base->ExistModifies(first_, index_2nd);
    return ResultTable(result);
}
ModifiesIntSyn::ModifiesIntSyn(int first, Synonym *second)
        : first_(first), second_(second) {}
ResultTable ModifiesIntSyn::Execute(KnowledgeBase *knowledge_base) const {
    auto result =
            knowledge_base->GetModifies(Index<QueryEntityType::kStmt>(first_));
    return {second_, std::move(result)};
}
ModifiesIntWild::ModifiesIntWild(int first) : first_(first) {}
ResultTable ModifiesIntWild::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->ExistModifies(first_, 0);
    return ResultTable(result);
}
ModifiesSynIdent::ModifiesSynIdent(Synonym *first, std::string second)
        : first_(first), second_(std::move(second)) {}
ResultTable ModifiesSynIdent::Execute(KnowledgeBase *knowledge_base) const {
    auto index_2nd =
            knowledge_base->NameToIndex(QueryEntityType::kVar, second_);
    if (index_2nd == 0) return ResultTable(false);
    auto result = knowledge_base->GetModifies(
            Index<QueryEntityType::kVar>(index_2nd), SynToPkbType(first_));
    return {first_, std::move(result)};
}
ModifiesSynSyn::ModifiesSynSyn(Synonym *first, Synonym *second)
        : first_(first), second_(second) {}
ResultTable ModifiesSynSyn::Execute(KnowledgeBase *knowledge_base) const {
    auto [col_1, col_2] =
            knowledge_base->GetModifiesStmtVar(SynToPkbType(first_));
    return {first_, std::move(col_1), second_, std::move(col_2)};
}
ModifiesSynWild::ModifiesSynWild(Synonym *first) : first_(first) {}
ResultTable ModifiesSynWild::Execute(KnowledgeBase *knowledge_base) const {
    auto result = knowledge_base->GetModifies(SynToPkbType(first_));
    return {first_, std::move(result)};
}
}  // namespace spa
