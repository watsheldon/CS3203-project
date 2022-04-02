#include "pattern_expr_base.h"

namespace spa {
ResultTable PatternExprBase::Execute(KnowledgeBase *pkb) const noexcept {
    switch (type_) {
        case kVarExpr: {
            auto first_var_name = std::get<VarName>(first_param_);
            auto second_expr = std::get<Expression>(second_param_);
            return VarExpr(pkb, first_var_name, second_expr);
        }
        case kSynExpr: {
            auto first_syn = std::get<Synonym *>(first_param_);
            auto second_expr = std::get<Expression>(second_param_);
            first_syn->IncResolved();
            return SynExpr(pkb, first_syn, second_expr);
        }
        case kWildExpr: {
            auto second_expr = std::get<Expression>(second_param_);
            return WildExpr(pkb, second_expr);
        }
    }
    assert(false);
    return ResultTable{false};
}
}  // namespace spa
