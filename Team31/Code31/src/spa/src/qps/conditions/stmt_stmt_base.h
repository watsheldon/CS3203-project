#ifndef SRC_SPA_SRC_QPS_CONDITIONS_STMTSTMTBASE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_STMTSTMTBASE_H_

#include <array>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "condition_clause.h"
#include "qps/synonym.h"

namespace spa {
class StmtStmtBase : public ConditionClause {
  public:
    enum Type {
        kIntInt,
        kIntSyn,
        kIntWild,
        kSynInt,
        kSynSyn,
        kSynWild,
        kWildInt,
        kWildSyn,
        kWildWild,
    };

    StmtStmtBase() noexcept;
    StmtStmtBase(int first, int second) noexcept;
    StmtStmtBase(int first, Synonym *second) noexcept;
    StmtStmtBase(Synonym *first, int second) noexcept;
    StmtStmtBase(Synonym *first, Synonym *second) noexcept;
    StmtStmtBase(ArgPos pos, int integer) noexcept;
    StmtStmtBase(ArgPos pos, Synonym *syn) noexcept;
    static constexpr Type GetType(FirstParamType first,
                                  SecondParamType second) noexcept {
        int first_index = static_cast<int>(first);
        int second_index = static_cast<int>(second);
        return kTypeMatrix[first_index][second_index];
    }
    ~StmtStmtBase() override = default;

  protected:
    Type type_;
    int first_int_ = 0;
    int second_int_ = 0;
    Synonym *first_syn_ = nullptr;
    Synonym *second_syn_ = nullptr;
    static constexpr Array2D<Type, 3, 3> kTypeMatrix{
            {{Type::kIntInt, Type::kIntSyn, Type::kIntWild},
             {Type::kSynInt, Type::kSynSyn, Type::kSynWild},
             {Type::kWildInt, Type::kWildSyn, Type::kWildWild}}};
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_STMTSTMTBASE_H_
