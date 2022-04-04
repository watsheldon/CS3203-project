#ifndef SRC_SPA_SRC_QPS_CONDITIONS_FACTORY_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_FACTORY_H_

#include <array>
#include <string>

#include "affects_clause.h"
#include "affects_trans_clause.h"
#include "calls_base.h"
#include "calls_clause.h"
#include "calls_trans_clause.h"
#include "condition_clause.h"
#include "follows_clause.h"
#include "follows_trans_clause.h"
#include "modifies_clause.h"
#include "next_clause.h"
#include "next_trans_clause.h"
#include "parent_clause.h"
#include "parent_trans_clause.h"
#include "pattern_assign_exact.h"
#include "pattern_assign_partial.h"
#include "pattern_base.h"
#include "pattern_expr_base.h"
#include "pattern_while_if_clause.h"
#include "qps/query_token.h"
#include "qps/synonym.h"
#include "stmt_stmt_base.h"
#include "uses_clause.h"
#include "uses_modifies_base.h"

namespace spa {

class Factory {
  public:
    void SetRelationship(QueryTokenType type) noexcept;
    void SetFirst(int first) noexcept;
    void SetFirst(Synonym* syn) noexcept;
    void SetFirst(const std::string& value) noexcept;
    void SetSecond(int second) noexcept;
    void SetSecond(Synonym* syn) noexcept;
    void SetSecond(const std::string& value) noexcept;
    void SetSecond(std::vector<QueryToken>&& expr) noexcept;
    void SetTransPartial() noexcept;
    void SetPatternSynonym(Synonym* syn) noexcept;
    std::unique_ptr<ConditionClause> Build() noexcept;

  private:
    enum Relationship {
        kFollows,
        kFollowsT,
        kParent,
        kParentT,
        kUses,
        kModifies,
        kPatternPartial,
        kPatternExact,
        kPatternWhile,
        kPatternIf,
        kCalls,
        kCallsT,
        kNext,
        kNextT,
        kAffects,
        kAffectsT
    };
    Relationship rel_;
    ConditionClause::FirstParamType first_param_type_;
    ConditionClause::SecondParamType second_param_type_;
    int first_int_;
    int second_int_;
    std::string_view first_ident_;
    std::string_view second_ident_;
    Synonym* syn_;
    Synonym* first_syn_;
    Synonym* second_syn_;
    std::vector<QueryToken> second_exprs_;
    template <typename T>
    std::unique_ptr<ConditionClause> BuildStmtStmtClause() noexcept {
        static_assert(std::is_base_of_v<StmtStmtBase, T>);
        StmtStmtBase::Type type =
                StmtStmtBase::GetType(first_param_type_, second_param_type_);
        switch (type) {
            case StmtStmtBase::Type::kNumNum:
                return std::make_unique<T>(first_int_, second_int_);
            case StmtStmtBase::Type::kNumSyn:
                return std::make_unique<T>(first_int_, second_syn_);
            case StmtStmtBase::Type::kNumWild:
                return std::make_unique<T>(first_int_,
                                           ConditionClause::Wildcard{});
            case StmtStmtBase::Type::kSynNum:
                return std::make_unique<T>(first_syn_, second_int_);
            case StmtStmtBase::Type::kSynSyn:
                if constexpr (std::is_base_of_v<OrderedStmtStmtBase, T> ||
                              std::is_same_v<NextClause, T>) {
                    if (first_syn_ == second_syn_) return nullptr;
                }
                return std::make_unique<T>(first_syn_, second_syn_);
            case StmtStmtBase::Type::kSynWild:
                return std::make_unique<T>(first_syn_,
                                           ConditionClause::Wildcard{});
            case StmtStmtBase::Type::kWildNum:
                return std::make_unique<T>(ConditionClause::Wildcard{},
                                           second_int_);
            case StmtStmtBase::Type::kWildSyn:
                return std::make_unique<T>(ConditionClause::Wildcard{},
                                           second_syn_);
            case StmtStmtBase::Type::kWildWild:
                return std::make_unique<T>();
        }
        assert(false);
        return {};
    }
    template <typename T>
    std::unique_ptr<ConditionClause> BuildUsesModifiesClause() noexcept {
        static_assert(std::is_base_of_v<UsesModifiesBase, T>);
        UsesModifiesBase::Type type = UsesModifiesBase::GetType(
                first_param_type_, second_param_type_);
        switch (type) {
            case UsesModifiesBase::Type::kNumSyn:
                return std::make_unique<T>(first_int_, second_syn_);
            case UsesModifiesBase::Type::kNumWild:
                return std::make_unique<T>(first_int_);
            case UsesModifiesBase::Type::kNumVar:
                return std::make_unique<T>(first_int_, second_ident_);
            case UsesModifiesBase::Type::kSynSyn:
                return std::make_unique<T>(first_syn_, second_syn_);
            case UsesModifiesBase::Type::kSynWild:
                return std::make_unique<T>(first_syn_);
            case UsesModifiesBase::Type::kSynVar:
                return std::make_unique<T>(first_syn_, second_ident_);
            case UsesModifiesBase::Type::kProcSyn:
                return std::make_unique<T>(first_ident_, second_syn_);
            case UsesModifiesBase::Type::kProcVar:
                return std::make_unique<T>(first_ident_, second_ident_);
            case UsesModifiesBase::Type::kProcWild:
                return std::make_unique<T>(first_ident_);
        }
        assert(false);
        return {};
    }
    template <typename T>
    std::unique_ptr<ConditionClause> BuildPatternExprClause() noexcept {
        static_assert(std::is_base_of_v<PatternExprBase, T>);
        PatternExprBase::Type type =
                PatternExprBase::GetType(first_param_type_, second_param_type_);
        switch (type) {
            case PatternBase::Type::kVarExpr:
                return std::make_unique<T>(syn_, first_ident_,
                                           std::move(second_exprs_));
            case PatternBase::Type::kVarWild:
                return std::make_unique<T>(syn_, first_ident_);
            case PatternBase::Type::kSynExpr:
                return std::make_unique<T>(syn_, first_syn_,
                                           std::move(second_exprs_));
            case PatternBase::Type::kSynWild:
                return std::make_unique<T>(syn_, first_syn_);
            case PatternBase::Type::kWildExpr:
                return std::make_unique<T>(syn_, std::move(second_exprs_));
            case PatternExprBase::Type::kWildWild:
                return std::make_unique<T>(syn_);
        }
        assert(false);
        return {};
    }
    template <typename T>
    std::unique_ptr<ConditionClause> BuildPatternWhileIfClause() noexcept {
        static_assert(std::is_base_of_v<PatternBase, T>);
        PatternBase::Type type = PatternBase::GetType(first_param_type_);
        switch (type) {
            case PatternBase::Type::kVarWild:
                return std::make_unique<T>(syn_, first_ident_);
            case PatternBase::Type::kSynWild:
                return std::make_unique<T>(syn_, first_syn_);
            case PatternBase::Type::kWildWild:
                return std::make_unique<T>(syn_);
            default:
                assert(false);
        }
        return {};
    }
    template <typename T>
    std::unique_ptr<ConditionClause> BuildCallsClause() noexcept {
        static_assert(std::is_base_of_v<CallsBase, T>);
        CallsBase::Type type =
                CallsBase::GetType(first_param_type_, second_param_type_);
        switch (type) {
            case CallsBase::Type::kSynSyn:
                return std::make_unique<T>(first_syn_, second_syn_);
            case CallsBase::Type::kSynWild:
                return std::make_unique<T>(first_syn_,
                                           ConditionClause::Wildcard{});
            case CallsBase::Type::kSynProc:
                return std::make_unique<T>(first_syn_, second_ident_);
            case CallsBase::Type::kWildSyn:
                return std::make_unique<T>(ConditionClause::Wildcard{},
                                           second_syn_);
            case CallsBase::Type::kWildWild:
                return std::make_unique<T>(ConditionClause::Wildcard{},
                                           ConditionClause::Wildcard{});
            case CallsBase::Type::kWildProc:
                return std::make_unique<T>(ConditionClause::Wildcard{},
                                           second_ident_);
            case CallsBase::Type::kProcSyn:
                return std::make_unique<T>(first_ident_, second_syn_);
            case CallsBase::Type::kProcWild:
                return std::make_unique<T>(first_ident_,
                                           ConditionClause::Wildcard{});
            case CallsBase::Type::kProcProc:
                return std::make_unique<T>(first_ident_, second_ident_);
        }
        assert(false);
        return {};
    }
    void Reset() noexcept;
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_FACTORY_H_
