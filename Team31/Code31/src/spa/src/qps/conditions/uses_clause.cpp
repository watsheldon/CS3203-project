#include "uses_clause.h"

#include "common/type_convert_helpers.h"
namespace spa {
ResultTable UsesClause::Execute(KnowledgeBase *knowledge_base) const {
    switch (type_) {
        case Type::kIntSyn: {
            auto result = knowledge_base->GetUses(
                    Index<QueryEntityType::kStmt>(first_int_));
            return {second_syn_, std::move(result)};
        }
        case Type::kIntWild: {
            auto result = knowledge_base->ExistUses(first_int_,
                                                    KnowledgeBase::kWildCard);
            return ResultTable(result);
        }
        case Type::kIntIdent: {
            auto result = knowledge_base->ExistUses(first_int_, second_ident_);
            return ResultTable(result);
        }
        case Type::kSynSyn: {
            auto [col_1, col_2] =
                    knowledge_base->GetUsesStmtVar(SynToPkbType(first_syn_));
            return {first_syn_, std::move(col_1), second_syn_,
                    std::move(col_2)};
        }
        case Type::kSynWild: {
            auto result = knowledge_base->GetUses(SynToPkbType(first_syn_));
            return {first_syn_, std::move(result)};
        }
        case Type::kSynIdent: {
            auto result = knowledge_base->GetUses(second_ident_,
                                                  SynToPkbType(first_syn_));
            return {first_syn_, std::move(result)};
        }
        default:
            assert(false);
    }
}
}  // namespace spa
