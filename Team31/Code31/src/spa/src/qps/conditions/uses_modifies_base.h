#ifndef SRC_SPA_SRC_QPS_CONDITIONS_USESMODIFIESBASE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_USESMODIFIESBASE_H_

#include <string>

#include "common/aliases.h"
#include "condition_clause.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"
#include "qps/synonym.h"

namespace spa {
class UsesModifiesBase : public ConditionClause {
  public:
    enum Type {
        kIntIdent,
        kIntSyn,
        kIntWild,
        kSynIdent,
        kSynSyn,
        kSynWild,
        kIdentSyn,
        kIdentIdent,
        kIdentWild
    };

    UsesModifiesBase(int first, std::string second) noexcept;
    UsesModifiesBase(int first, Synonym *second) noexcept;
    explicit UsesModifiesBase(int first) noexcept;
    UsesModifiesBase(Synonym *first, std::string second) noexcept;
    UsesModifiesBase(Synonym *first, Synonym *second) noexcept;
    explicit UsesModifiesBase(Synonym *first) noexcept;
    UsesModifiesBase(std::string first, Synonym *second) noexcept;
    UsesModifiesBase(std::string first, std::string second) noexcept;
    explicit UsesModifiesBase(std::string first) noexcept;
    static constexpr Type GetType(FirstParamType first,
                                  SecondParamType second) noexcept {
        int first_index = first == FirstParamType::kIdent
                                  ? static_cast<int>(first) - 1
                                  : static_cast<int>(first);
        int second_index = second == SecondParamType::kIdent
                                   ? static_cast<int>(second) - 2
                                   : static_cast<int>(second) - 1;
        return uses_modifies_type_[first_index][second_index];
    }
    ~UsesModifiesBase() override = default;

  protected:
    Type type_;
    int first_int_ = 0;
    Synonym *first_syn_ = nullptr;
    std::string first_ident_;
    int second_int_ = 0;
    Synonym *second_syn_ = nullptr;
    std::string second_ident_;
    static constexpr Array2D<Type, 3, 3> uses_modifies_type_{
            {{Type::kIntSyn, Type::kIntWild, Type::kIntIdent},
             {Type::kSynSyn, Type::kSynWild, Type::kSynIdent},
             {Type::kIdentSyn, Type::kIdentWild, Type::kIdentIdent}}};
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_USESMODIFIESBASE_H_
