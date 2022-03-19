#include "factory.h"

#include <cassert>

#include "pattern_clause.h"
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
    first_index_ = 1;
}
void Factory::SetFirst(Synonym* syn) {
    first_syn_ = syn;
    first_index_ = 2;
}
void Factory::SetFirst(const std::string& value) {
    first_ident_ = value;
    switch (rel_) {
        case kPatternExact:
        case kPatternPartial:
            first_index_ = 1;
            return;
        case kUses:
        case kModifies:
            first_index_ = 0;
            return;
        default:
            assert(false);
    }
}
void Factory::SetSecond(int second) {
    second_int_ = second;
    second_index_ = 1;
}
void Factory::SetSecond(Synonym* syn) {
    second_syn_ = syn;
    second_index_ = 2;
}
void Factory::SetSecond(const std::string& value) {
    second_ident_ = value;
    second_index_ = 1;
}
void Factory::SetSecond(std::vector<QueryToken>&& expr) {
    second_exprs_ = std::move(expr);
    second_index_ = 1;
}
void Factory::SetTransPartial() {
    switch (rel_) {
        case Relationship::kFollows:
            rel_ = Relationship::kFollowsTrans;
            return;
        case Relationship::kParent:
            rel_ = Relationship::kParentTrans;
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
            param_type_ = stmt_stmt_type_.at(first_index_).at(second_index_);
            return BuildStmtStmtClause<ParentClause>();
        case Relationship::kParentTrans:
            param_type_ = stmt_stmt_type_.at(first_index_).at(second_index_);
            return BuildStmtStmtClause<ParentTransClause>();
        case Relationship::kFollows:
            param_type_ = stmt_stmt_type_.at(first_index_).at(second_index_);
            return BuildStmtStmtClause<FollowsClause>();
        case Relationship::kFollowsTrans:
            param_type_ = stmt_stmt_type_.at(first_index_).at(second_index_);
            return BuildStmtStmtClause<FollowsTransClause>();
        case Relationship::kUses:
            param_type_ =
                    uses_modifies_type_.at(first_index_).at(second_index_);
            return BuildUsesModifiesClause<UsesClause>();
        case Relationship::kModifies:
            param_type_ =
                    uses_modifies_type_.at(first_index_).at(second_index_);
            return BuildUsesModifiesClause<ModifiesClause>();
        case Relationship::kPatternExact:
            param_type_ = pattern_type_.at(first_index_).at(second_index_);
            return BuildPatternClause<PatternExact>();
        case Relationship::kPatternPartial:
            param_type_ = pattern_type_.at(first_index_).at(second_index_);
            return BuildPatternClause<PatternPartial>();
        default:
            assert(false);
    }
}

void Factory::Reset() {
    rel_ = Relationship::kNone;
    param_type_ = ParamType::kNoneNone;
    first_int_ = 0;
    second_int_ = 0;
    first_index_ = 0;
    second_index_ = 0;
    first_ident_.clear();
    second_ident_.clear();
    assign_ = nullptr;
    first_syn_ = nullptr;
    second_syn_ = nullptr;
    second_exprs_.clear();
}
}  // namespace spa
