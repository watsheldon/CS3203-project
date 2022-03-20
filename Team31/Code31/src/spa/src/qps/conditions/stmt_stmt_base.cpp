#include "stmt_stmt_base.h"

#include "common/type_convert_helpers.h"

namespace spa {
StmtStmtBase::StmtStmtBase() : type_(Type::kWildWild) {}
StmtStmtBase::StmtStmtBase(int first, int second)
        : type_(Type::kIntInt), first_int_(first), second_int_(second) {}
StmtStmtBase::StmtStmtBase(int first, Synonym *second)
        : type_(Type::kIntSyn),
          first_int_(first),
          second_syn_(std::move(second)) {}
StmtStmtBase::StmtStmtBase(Synonym *first, int second)
        : type_(Type::kSynInt),
          first_syn_(std::move(first)),
          second_int_(second) {}
StmtStmtBase::StmtStmtBase(Synonym *first, Synonym *second)
        : type_(Type::kSynSyn),
          first_syn_(std::move(first)),
          second_syn_(std::move(second)) {}
StmtStmtBase::StmtStmtBase(ArgPos pos, int integer) {
    if (pos == ArgPos::kFirst) {
        type_ = Type::kIntWild;
        first_int_ = integer;
    } else {
        type_ = Type::kWildInt;
        second_int_ = integer;
    }
}
StmtStmtBase::StmtStmtBase(ArgPos pos, Synonym *syn) {
    if (pos == ArgPos::kFirst) {
        type_ = Type::kSynWild;
        first_syn_ = syn;
    } else {
        type_ = Type::kWildSyn;
        second_syn_ = syn;
    }
}
StmtStmtBase::Type StmtStmtBase::StmtStmtType(
        ConditionClause::FirstParamType first,
        ConditionClause::SecondParamType second) {
    int first_index = static_cast<int>(first);
    int second_index = static_cast<int>(second);
    return stmt_stmt_type_[first_index][second_index];
}
}  // namespace spa
