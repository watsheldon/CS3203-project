#include "uses_modifies_base.h"

#include <string>
#include <utility>

#include "condition_clause.h"
#include "qps/synonym.h"

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
UsesModifiesBase::Type UsesModifiesBase::UsesModifiesType(
        ConditionClause::FirstParamType first,
        ConditionClause::SecondParamType second) {
    int first_index = first == FirstParamType::kIdent
                              ? static_cast<int>(first) - 1
                              : static_cast<int>(first);
    int second_index = second == SecondParamType::kIdent
                               ? static_cast<int>(second) - 2
                               : static_cast<int>(second) - 1;
    return uses_modifies_type_[first_index][second_index];
}
}  // namespace spa
