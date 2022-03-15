#include "uses_modifies_base.h"

#include "common/type_convert_helpers.h"
namespace spa {
UsesModifiesBase::UsesModifiesBase(int first, std::string second)
        : type_(Type::kIntIdent),
          first_int_(first),
          second_ident_(std::move(second)) {}
UsesModifiesBase::UsesModifiesBase(int first, Synonym *second)
        : type_(Type::kIntSyn), first_int_(first), second_syn_(second) {}
UsesModifiesBase::UsesModifiesBase(int first)
        : type_(Type::kIntWild), first_int_(first) {}
UsesModifiesBase::UsesModifiesBase(Synonym *first, std::string second)
        : type_(Type::kSynIdent),
          first_syn_(first),
          second_ident_(std::move(second)) {}
UsesModifiesBase::UsesModifiesBase(Synonym *first, Synonym *second)
        : type_(Type::kSynSyn), first_syn_(first), second_syn_(second) {}
UsesModifiesBase::UsesModifiesBase(Synonym *first)
        : type_(Type::kSynWild), first_syn_(first) {}
UsesModifiesBase::UsesModifiesBase(std::string first, Synonym *second)
        : type_(Type::kIdentSyn),
          first_ident_(std::move(first)),
          second_syn_(second) {}
UsesModifiesBase::UsesModifiesBase(std::string first, std::string second)
        : type_(Type::kIdentIdent),
          first_ident_(std::move(first)),
          second_ident_(std::move(second)) {}
UsesModifiesBase::UsesModifiesBase(std::string first)
        : type_(Type::kIdentWild), first_ident_(std::move(first)) {}

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
        case Type::kIdentIdent:
            break;
        case Type::kIdentSyn:
            break;
        case Type::kIdentWild:
            break;
    }
}
ResultTable ModifiesClause::Execute(KnowledgeBase *knowledge_base) const {
    switch (type_) {
        case Type::kIntSyn: {
            auto result = knowledge_base->GetModifies(
                    Index<QueryEntityType::kStmt>(first_int_));
            return {second_syn_, std::move(result)};
        }
        case Type::kIntWild: {
            auto result = knowledge_base->ExistModifies(
                    first_int_, KnowledgeBase::kWildCard);
            return ResultTable(result);
        }
        case Type::kIntIdent: {
            auto result =
                    knowledge_base->ExistModifies(first_int_, second_ident_);
            return ResultTable(result);
        }
        case Type::kSynSyn: {
            auto [col_1, col_2] = knowledge_base->GetModifiesStmtVar(
                    SynToPkbType(first_syn_));
            return {first_syn_, std::move(col_1), second_syn_,
                    std::move(col_2)};
        }
        case Type::kSynWild: {
            auto result = knowledge_base->GetModifies(SynToPkbType(first_syn_));
            return {first_syn_, std::move(result)};
        }
        case Type::kSynIdent: {
            auto result = knowledge_base->GetModifies(second_ident_,
                                                      SynToPkbType(first_syn_));
            return {first_syn_, std::move(result)};
        }
        case Type::kIdentIdent:
            break;
        case Type::kIdentSyn:
            break;
        case Type::kIdentWild:
            break;
    }
}
}  // namespace spa
