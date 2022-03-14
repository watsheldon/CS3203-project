#include "modifies_clause.h"

#include "common/entity_type_enum.h"
#include "common/index.h"
#include "common/type_convert_helpers.h"

namespace spa {
ModifiesClause::ModifiesClause(int first, std::string second)
        : type_(Type::kIntIdent),
          first_int_(first),
          second_ident_(std::move(second)) {}
ModifiesClause::ModifiesClause(int first, Synonym *second)
        : type_(Type::kIntSyn), first_int_(first), second_syn_(second) {}
ModifiesClause::ModifiesClause(int first)
        : type_(Type::kIntWild), first_int_(first) {}
ModifiesClause::ModifiesClause(Synonym *first, std::string second)
        : type_(Type::kSynIdent),
          first_syn_(first),
          second_ident_(std::move(second)) {}
ModifiesClause::ModifiesClause(Synonym *first, Synonym *second)
        : type_(Type::kSynSyn), first_syn_(first), second_syn_(second) {}
ModifiesClause::ModifiesClause(Synonym *first)
        : type_(Type::kSynWild), first_syn_(first) {}
ModifiesClause::ModifiesClause(std::string first, Synonym *second)
        : type_(Type::kIdentSyn),
          first_ident_(std::move(first)),
          second_syn_(second) {}
ModifiesClause::ModifiesClause(std::string first, std::string second)
        : type_(Type::kIdentIdent),
          first_ident_(std::move(first)),
          second_ident_(std::move(second)) {}
ModifiesClause::ModifiesClause(std::string first)
        : type_(Type::kIdentWild), first_ident_(std::move(first)) {}

ResultTable ModifiesClause::Execute(KnowledgeBase *knowledge_base) const {
    switch (type_) {
        case Type::kIntSyn: {
            auto result = knowledge_base->GetModifies(
                    Index<QueryEntityType::kStmt>(first_int_));
            return {second_syn_, std::move(result)};
        }
        case Type::kIntWild: {
            auto result = knowledge_base->ExistModifies(first_int_, 0);
            return ResultTable(result);
        }
        case Type::kIntIdent: {
            auto index_2nd = knowledge_base->NameToIndex(QueryEntityType::kVar,
                                                         second_ident_);
            if (index_2nd == 0) return ResultTable(false);
            auto result = knowledge_base->ExistModifies(first_int_, index_2nd);
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
            auto index_2nd = knowledge_base->NameToIndex(QueryEntityType::kVar,
                                                         second_ident_);
            if (index_2nd == 0) return ResultTable(false);
            auto result = knowledge_base->GetModifies(
                    Index<QueryEntityType::kVar>(index_2nd),
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
