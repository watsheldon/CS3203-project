#include "factory.h"

#include <cassert>

#include "pattern_base.h"
#include "qps/query_token.h"
#include "stmt_stmt_base.h"

namespace spa {
void Factory::SetRelationship(QueryTokenType type) {
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
        default:
            assert(false);
    }
}
void Factory::SetFirst(int first) {
    first_int_ = first;
    first_param_type_ = ConditionClause::FirstParamType::kInt;
}
void Factory::SetFirst(Synonym* syn) {
    first_syn_ = syn;
    first_param_type_ = ConditionClause::FirstParamType::kSyn;
}
void Factory::SetFirst(const std::string& value) {
    first_ident_ = value;
    first_param_type_ = ConditionClause::FirstParamType::kIdent;
}
void Factory::SetSecond(int second) {
    second_int_ = second;
    second_param_type_ = ConditionClause::SecondParamType::kInt;
}
void Factory::SetSecond(Synonym* syn) {
    second_syn_ = syn;
    second_param_type_ = ConditionClause::SecondParamType::kSyn;
}
void Factory::SetSecond(const std::string& value) {
    second_ident_ = value;
    second_param_type_ = ConditionClause::SecondParamType::kIdent;
}
void Factory::SetSecond(std::vector<QueryToken>&& expr) {
    second_exprs_ = std::move(expr);
    second_param_type_ = ConditionClause::SecondParamType::kExpr;
}
void Factory::SetTransPartial() {
    switch (rel_) {
        case Relationship::kFollows:
            rel_ = Relationship::kFollowsT;
            return;
        case Relationship::kParent:
            rel_ = Relationship::kParentT;
            return;
        case Relationship::kPatternExact:
        case Relationship::kPatternPartial:
            rel_ = Relationship::kPatternPartial;
            return;
        default:
            assert(false);
    }
}
void Factory::SetAssign(Synonym* syn) { assign_ = syn; }
std::unique_ptr<ConditionClause> Factory::Build() {
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
            return BuildPatternClause<PatternExactClause>();
        case Relationship::kPatternPartial:
            return BuildPatternClause<PatternPartialClause>();
        default:
            assert(false);
    }
}

void Factory::Reset() {
    first_int_ = 0;
    second_int_ = 0;
    first_param_type_ = ConditionClause::FirstParamType::kWild;
    second_param_type_ = ConditionClause::SecondParamType::kWild;
    first_ident_.clear();
    second_ident_.clear();
    assign_ = nullptr;
    first_syn_ = nullptr;
    second_syn_ = nullptr;
    second_exprs_.clear();
}
}  // namespace spa
