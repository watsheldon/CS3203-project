#include "factory.h"

#include <cassert>

#include "follows_clause.h"
#include "modifies_clause.h"
#include "parent_clause.h"
#include "pattern_clause.h"
#include "uses_clause.h"

namespace spa {
void Factory::SetRelationship(QueryTokenType type) {
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
            rel_ = Relationship::kPattern;
            return;
        default:
            assert(false);
    }
}
void Factory::SetFirst(int first) { first_int_ = first; }
void Factory::SetFirst(Synonym* syn) { first_syn_ = syn; }
void Factory::SetFirst(const std::string& value) { first_ident_ = value; }
void Factory::SetSecond(int second) { second_int_ = second; }
void Factory::SetSecond(Synonym* syn) { second_syn_ = syn; }
void Factory::SetSecond(const std::string& value) { second_ident_ = value; }
void Factory::SetSecond(std::vector<QueryToken>& expr) {
    second_exprs_ = std::move(expr);
}
void Factory::SetTrans(bool isTrans) { is_transitive = isTrans; }
void Factory::SetAssign(Synonym* syn) { assign_ = syn; }
std::unique_ptr<ConditionClause> Factory::Build() {
    switch (rel_) {
        case Relationship::kParent:
            if (first_int_) {
                if (second_int_) {
                    return std::make_unique<ParentIntInt>(
                            first_int_, second_int_, is_transitive);
                }

                if (second_syn_) {
                    return std::make_unique<ParentIntSyn>(
                            first_int_, second_syn_, is_transitive);
                }
                return std::make_unique<ParentIntWild>(first_int_,
                                                       is_transitive);
            }
            if (first_syn_) {
                if (second_int_) {
                    return std::make_unique<ParentSynInt>(
                            first_syn_, second_int_, is_transitive);
                }
                if (second_syn_) {
                    return std::make_unique<ParentSynSyn>(
                            first_syn_, second_syn_, is_transitive);
                }
                return std::make_unique<ParentSynWild>(first_syn_,
                                                       is_transitive);
            }

            if (second_int_) {
                return std::make_unique<ParentWildInt>(second_int_,
                                                       is_transitive);
            }

            if (second_syn_) {
                return std::make_unique<ParentWildSyn>(second_syn_,
                                                       is_transitive);
            }
            return std::make_unique<ParentWildWild>();

        case Relationship::kFollows:
            if (first_int_) {
                if (second_int_) {
                    return std::make_unique<FollowsIntInt>(
                            first_int_, second_int_, is_transitive);
                }
                if (second_syn_) {
                    return std::make_unique<FollowsIntSyn>(
                            first_int_, second_syn_, is_transitive);
                }
                return std::make_unique<FollowsIntWild>(first_int_,
                                                        is_transitive);
            }
            if (first_syn_) {
                if (second_int_) {
                    return std::make_unique<FollowsSynInt>(
                            first_syn_, second_int_, is_transitive);
                }
                if (second_syn_) {
                    return std::make_unique<FollowsSynSyn>(
                            first_syn_, second_syn_, is_transitive);
                }
                return std::make_unique<FollowsSynWild>(first_syn_,
                                                        is_transitive);
            }

            if (second_int_) {
                return std::make_unique<FollowsWildInt>(second_int_,
                                                        is_transitive);
            }

            if (second_syn_) {
                return std::make_unique<FollowsWildSyn>(second_syn_,
                                                        is_transitive);
            }
            return std::make_unique<FollowsWildWild>();

        case Relationship::kUses:
            if (first_int_) {
                if (second_syn_) {
                    return std::make_unique<UsesIntSyn>(first_int_,
                                                        second_syn_);
                }
                if (second_ident_.empty()) {
                    return std::make_unique<UsesIntWild>(first_int_);
                }
                return std::make_unique<UsesIntIdent>(first_int_,
                                                      second_ident_);
            }
            if (second_syn_) {
                return std::make_unique<UsesSynSyn>(first_syn_, second_syn_);
            }
            if (second_ident_.empty()) {
                return std::make_unique<UsesSynWild>(first_syn_);
            }
            return std::make_unique<UsesSynIdent>(first_syn_, second_ident_);

        case Relationship::kModifies:
            if (first_int_) {
                if (second_syn_) {
                    return std::make_unique<ModifiesIntSyn>(first_int_,
                                                            second_syn_);
                }
                if (second_ident_.empty()) {
                    return std::make_unique<ModifiesIntWild>(first_int_);
                }
                return std::make_unique<ModifiesIntIdent>(first_int_,
                                                          second_ident_);
            }
            if (second_syn_) {
                return std::make_unique<ModifiesSynSyn>(first_syn_,
                                                        second_syn_);
            }
            if (second_ident_.empty()) {
                return std::make_unique<ModifiesSynWild>(first_syn_);
            }
            return std::make_unique<ModifiesSynIdent>(first_syn_,
                                                      second_ident_);

        case Relationship::kPattern:
            assert(assign_);
            if (first_syn_) {
                if (second_exprs_.empty()) {
                    return std::make_unique<PatternSynWild>(first_syn_);
                }
                return std::make_unique<PatternSynExpr>(first_syn_,
                                                        second_exprs_);
            }
            if (first_ident_.empty()) {
                if (second_exprs_.empty()) {
                    return std::make_unique<ParentWildWild>();
                }
                return std::make_unique<PatternWildExpr>(second_exprs_);
            }
            if (second_exprs_.empty()) {
                return std::make_unique<PatternIdentWild>(first_ident_);
            }
            return std::make_unique<PatternIdentExpr>(first_ident_,
                                                      second_exprs_);
        case Relationship::kNone:
            break;
    }
    return nullptr;
};
}  // namespace spa
