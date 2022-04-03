#include "pattern_expr_base.h"

namespace spa {
PatternExprBase::PatternExprBase(Synonym *assign, VarName first,
                                 std::vector<QueryToken> &&second) noexcept
        : PatternBase(Type::kVarExpr, assign, first),
          second_param_(std::move(second)) {}
PatternExprBase::PatternExprBase(Synonym *assign, Synonym *first,
                                 std::vector<QueryToken> &&second) noexcept
        : PatternBase(Type::kSynExpr, assign, first),
          second_param_(std::move(second)) {}
PatternExprBase::PatternExprBase(Synonym *assign,
                                 std::vector<QueryToken> &&second) noexcept
        : PatternBase(Type::kWildExpr, assign),
          second_param_(std::move(second)) {}
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
        case kWildWild:
        case kSynWild:
        case kVarWild:
            return PatternBase::Execute(pkb);
        default:
            assert(false);
    }
    return ResultTable{false};
}
}  // namespace spa
