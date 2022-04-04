#include "factory.h"

#include <cassert>

#include "qps/query_token.h"
#include "stmt_stmt_base.h"

namespace spa {
void Factory::SetRelationship(QueryTokenType type) noexcept {
    Reset();
    switch (type) {
        case QueryTokenType::kKeywordParent:
            rel_ = Relationship::kParent;
            return;
        case QueryTokenType::kKeywordFollows:
            rel_ = Relationship::kFollows;
            return;
        case QueryTokenType::kKeywordUses:
            rel_ = Relationship::kUses;
            return;
        case QueryTokenType::kKeywordModifies:
            rel_ = Relationship::kModifies;
            return;
        case QueryTokenType::kKeywordPattern:
            rel_ = Relationship::kPatternExact;
            return;
        case QueryTokenType::kKeywordCalls:
            rel_ = Relationship::kCalls;
            return;
        case QueryTokenType::kKeywordNext:
            rel_ = Relationship::kNext;
            return;
        case QueryTokenType::kKeywordAffects:
            rel_ = Relationship::kAffects;
            return;
        case QueryTokenType::kKeywordWith:
            rel_ = Relationship::kWith;
            return;
        default:
            assert(false);
    }
}
void Factory::SetFirst(int first) noexcept {
    first_int_ = first;
    first_param_type_ = ConditionClause::FirstParamType::kInt;
}
void Factory::SetFirst(Synonym* syn) noexcept {
    first_syn_ = syn;
    first_param_type_ = ConditionClause::FirstParamType::kSyn;
}
void Factory::SetFirst(const std::string& value) noexcept {
    first_ident_ = value;
    first_param_type_ = ConditionClause::FirstParamType::kIdent;
}
void Factory::SetFirst(SynonymWithAttr* syn_with_attr) noexcept {
    first_syn_attr_ = syn_with_attr;
    first_param_type_ = ConditionClause::FirstParamType::kSyn;
}
void Factory::SetSecond(int second) noexcept {
    second_int_ = second;
    second_param_type_ = ConditionClause::SecondParamType::kInt;
}
void Factory::SetSecond(Synonym* syn) noexcept {
    second_syn_ = syn;
    second_param_type_ = ConditionClause::SecondParamType::kSyn;
}
void Factory::SetSecond(const std::string& value) noexcept {
    second_ident_ = value;
    second_param_type_ = ConditionClause::SecondParamType::kIdent;
}
void Factory::SetSecond(std::vector<QueryToken>&& expr) noexcept {
    second_exprs_ = std::move(expr);
    second_param_type_ = ConditionClause::SecondParamType::kExpr;
}
void Factory::SetSecond(SynonymWithAttr* syn_with_attr) noexcept {
    second_syn_attr_ = syn_with_attr;
    second_param_type_ = ConditionClause::SecondParamType::kSyn;
}
void Factory::SetTransPartial() noexcept {
    switch (rel_) {
        case Relationship::kFollows:
            rel_ = Relationship::kFollowsT;
            return;
        case Relationship::kParent:
            rel_ = Relationship::kParentT;
            return;
        case Relationship::kCalls:
            rel_ = Relationship::kCallsT;
            return;
        case Relationship::kNext:
            rel_ = Relationship::kNextT;
            return;
        case Relationship::kAffects:
            rel_ = Relationship::kAffectsT;
            return;
        case Relationship::kPatternExact:
            rel_ = Relationship::kPatternPartial;
            return;
        case Relationship::kPatternPartial:
        case Relationship::kPatternIf:
        case Relationship::kPatternWhile:
            return;
        default:
            assert(false);
    }
}
void Factory::SetPatternSynonym(Synonym* syn) noexcept {
    syn_ = syn;
    switch (syn->type) {
        case Synonym::kStmtIf:
            rel_ = Relationship::kPatternIf;
            return;
        case Synonym::kStmtWhile:
            rel_ = Relationship::kPatternWhile;
            return;
        case Synonym::kStmtAssign:
            return;
        default:
            assert(false);
    }
}
std::unique_ptr<ConditionClause> Factory::Build() noexcept {
    switch (rel_) {
        case Relationship::kParent:
            return BuildStmtStmtClause<ParentClause>();
        case Relationship::kParentT:
            return BuildStmtStmtClause<ParentTransClause>();
        case Relationship::kFollows:
            return BuildStmtStmtClause<FollowsClause>();
        case Relationship::kFollowsT:
            return BuildStmtStmtClause<FollowsTransClause>();
        case Relationship::kUses:
            return BuildUsesModifiesClause<UsesClause>();
        case Relationship::kModifies:
            return BuildUsesModifiesClause<ModifiesClause>();
        case Relationship::kPatternExact:
            return BuildPatternExprClause<PatternExactClause>();
        case Relationship::kPatternPartial:
            return BuildPatternExprClause<PatternPartialClause>();
        case Relationship::kPatternWhile:
        case Relationship::kPatternIf:
            return BuildPatternWhileIfClause<PatternWhileIfClause>();
        case Relationship::kCalls:
            return BuildCallsClause<CallsClause>();
        case Relationship::kCallsT:
            return BuildCallsClause<CallsTransClause>();
        case Relationship::kNext:
            return BuildStmtStmtClause<NextClause>();
        case Relationship::kNextT:
            return BuildStmtStmtClause<NextTransClause>();
        case Relationship::kAffects:
            return BuildStmtStmtClause<AffectsClause>();
        case Relationship::kAffectsT:
            return BuildStmtStmtClause<AffectsTransClause>();
    }
    assert(false);
    return {};
}

void Factory::Reset() noexcept {
    first_param_type_ = ConditionClause::FirstParamType::kWild;
    second_param_type_ = ConditionClause::SecondParamType::kWild;
    second_exprs_.clear();
}
}  // namespace spa
