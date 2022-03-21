#ifndef SRC_SPA_SRC_QPS_CONDITIONS_CALLBASE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_CALLBASE_H_

#include <array>
#include <string>

#include "common/aliases.h"
#include "condition_clause.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"
#include "qps/synonym.h"

namespace spa {
class CallsBase : public ConditionClause {
  public:
    enum Type {
        kSynSyn,
        kSynWild,
        kSynIdent,
        kWildSyn,
        kWildWild,
        kWildIdent,
        kIdentSyn,
        kIdentWild,
        kIdentIdent;
    };

    CallsBase(Synonym *first, Synonym *second) noexcept;
    CallsBase(ArgPos pos, Synonym *syn) noexcept;
    CallsBase(Synonym *first, std::string second) noexcept;
    CallsBase(std::string first, Synonym *second) noexcept;
    CallsBase(ArgPos pos, std::string ident) noexcept;
    CallsBase(std::string first, std::string second) noexcept;
    CallsBase();
    static constexpr Type GetType(FirstParamType first,
                                  SecondParamType second) noexcept {
        int first_index = static_cast<int>(first) - 1;
        int second_index = second == SecondParamType::kIdent
                                   ? static_cast<int>(second) - 2
                                   : static_cast<int>(second) - 1;
        return kTypeMatrix[first_index][second_index];
    }

  protected:
    Type type_;
    Synonym *first_syn_ = nullptr;
    std::string first_ident_;
    Synonym *second_syn_ = nullptr;
    std::string second_ident_;
    static constexpr Array2D<Type, 3, 3> kTypeMatrix{
            {{Type::kSynSyn, Type::kSynWild, Type::kSynIdent},
             {Type::kWildSyn, Type::kWildWild, Type::kWildIdent},
             {Type::kIdentSyn, Type::kIdentWild, Type::kIdentIdent}}};
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_CALLBASE_H_
