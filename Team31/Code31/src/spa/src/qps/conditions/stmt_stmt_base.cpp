#include "stmt_stmt_base.h"

#include "common/entity_type_enum.h"
#include "qps/synonym.h"

namespace spa {
StmtStmtBase::StmtStmtBase() noexcept : type_(Type::kWildWild) {}
StmtStmtBase::StmtStmtBase(int first, int second) noexcept
        : type_(Type::kIntInt), first_int_(first), second_int_(second) {}
StmtStmtBase::StmtStmtBase(int first, Synonym *second) noexcept
        : type_(Type::kIntSyn), first_int_(first), second_syn_(second) {}
StmtStmtBase::StmtStmtBase(Synonym *first, int second) noexcept
        : type_(Type::kSynInt), first_syn_(first), second_int_(second) {}
StmtStmtBase::StmtStmtBase(Synonym *first, Synonym *second) noexcept
        : type_(Type::kSynSyn), first_syn_(first), second_syn_(second) {}
StmtStmtBase::StmtStmtBase(ArgPos pos, int integer) noexcept {
    if (pos == ArgPos::kFirst) {
        type_ = Type::kIntWild;
        first_int_ = integer;
    } else {
        type_ = Type::kWildInt;
        second_int_ = integer;
    }
}
StmtStmtBase::StmtStmtBase(ArgPos pos, Synonym *syn) noexcept {
    if (pos == ArgPos::kFirst) {
        type_ = Type::kSynWild;
        first_syn_ = syn;
    } else {
        type_ = Type::kWildSyn;
        second_syn_ = syn;
    }
}
}  // namespace spa
