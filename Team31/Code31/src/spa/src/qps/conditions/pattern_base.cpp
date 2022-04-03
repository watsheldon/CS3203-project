#include "pattern_base.h"

#include <string>
#include <utility>
#include <vector>

#include "qps/query_token.h"
#include "qps/synonym.h"

namespace spa {
PatternBase::PatternBase(Synonym *assign, VarName first) noexcept
        : type_(Type::kVarWild), zeroth_param_(assign), first_param_(first) {}
PatternBase::PatternBase(Synonym *assign, Synonym *first) noexcept
        : type_(Type::kSynWild), zeroth_param_(assign), first_param_(first) {}
PatternBase::PatternBase(Synonym *assign) noexcept
        : type_(Type::kWildWild), zeroth_param_(assign) {}
PatternBase::PatternBase(Type type, Synonym *zeroth,
                         FirstParam first_param) noexcept
        : type_(type), zeroth_param_(zeroth), first_param_(first_param) {}
ResultTable PatternBase::Execute(KnowledgeBase *pkb) const noexcept {
    switch (type_) {
        case kVarWild: {
            auto first_var_name = std::get<VarName>(first_param_);
            return VarWild(pkb, first_var_name);
        }
        case kSynWild: {
            auto first_syn = std::get<Synonym *>(first_param_);
            first_syn->IncResolved();
            return SynWild(pkb, first_syn);
        }
        case kWildWild: {
            return WildWild(pkb);
        }
        default:
            assert(false);
    }
    return ResultTable{false};
}
ResultTable PatternBase::VarWild(KnowledgeBase *pkb,
                                 VarName first) const noexcept {
    auto result = pkb->GetPattern(first);
    return {zeroth_param_, std::move(result)};
}
ResultTable PatternBase::SynWild(KnowledgeBase *pkb,
                                 Synonym *first) const noexcept {
    auto [col_1, col_2] = pkb->GetPatternPair();
    return {zeroth_param_, std::move(col_1), first, std::move(col_2)};
}
ResultTable PatternBase::WildWild(KnowledgeBase *pkb) const noexcept {
    auto result = pkb->GetAllEntityIndices(StmtType::kAssign);
    return {zeroth_param_, {result.begin(), result.end()}};
}
}  // namespace spa
