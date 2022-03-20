#ifndef SRC_SPA_SRC_QPS_CONDITIONS_FACTORY_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_FACTORY_H_

#include <array>
#include <string>

#include "affects_clause.h"
#include "affects_trans_clause.h"
#include "call_base.h"
#include "condition_clause.h"
#include "follows_clause.h"
#include "follows_trans_clause.h"
#include "modifies_clause.h"
#include "next_clause.h"
#include "next_trans_clause.h"
#include "parent_clause.h"
#include "parent_trans_clause.h"
#include "pattern_base.h"
#include "pattern_exact_clause.h"
#include "pattern_partial_clause.h"
#include "qps/query_token.h"
#include "qps/synonym.h"
#include "stmt_stmt_base.h"
#include "uses_clause.h"
#include "uses_modifies_base.h"

namespace spa {

class Factory {
  public:
    void SetRelationship(QueryTokenType type);
    void SetFirst(int first);
    void SetFirst(Synonym* syn);
    void SetFirst(const std::string& value);
    void SetSecond(int second);
    void SetSecond(Synonym* syn);
    void SetSecond(const std::string& value);
    void SetSecond(std::vector<QueryToken>&& expr);
    void SetTransPartial();
    void SetAssign(Synonym* syn);
    std::unique_ptr<ConditionClause> Build();

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
        kNext,
        kNextT,
        kAffects,
        kAffectsT,
        kCalls,
        kCallsT
    };
    Relationship rel_;
    ConditionClause::FirstParamType first_param_type_;
    ConditionClause::SecondParamType second_param_type_;
    int first_int_;
    int second_int_;
    std::string first_ident_;
    std::string second_ident_;
    Synonym* assign_;
    Synonym* first_syn_;
    Synonym* second_syn_;
    std::vector<QueryToken> second_exprs_;
    template <typename T>
    std::unique_ptr<ConditionClause> BuildStmtStmtClause() {
        static_assert(std::is_base_of_v<StmtStmtBase, T>);
        StmtStmtBase::Type type = StmtStmtBase::StmtStmtType(
                first_param_type_, second_param_type_);
        switch (type) {
            case StmtStmtBase::Type::kIntInt:
                return std::make_unique<T>(first_int_, second_int_);
            case StmtStmtBase::Type::kIntSyn:
                return std::make_unique<T>(first_int_, second_syn_);
            case StmtStmtBase::Type::kIntWild:
                return std::make_unique<T>(ArgPos::kFirst, first_int_);
            case StmtStmtBase::Type::kSynInt:
                return std::make_unique<T>(first_syn_, second_int_);
            case StmtStmtBase::Type::kSynSyn:
                if constexpr (std::is_base_of_v<OrderedStmtStmtBase, T>) {
                    if (first_syn_ == second_syn_) return nullptr;
                }
                return std::make_unique<T>(first_syn_, second_syn_);
            case StmtStmtBase::Type::kSynWild:
                return std::make_unique<T>(ArgPos::kFirst, first_syn_);
            case StmtStmtBase::Type::kWildInt:
                return std::make_unique<T>(ArgPos::kSecond, second_int_);
            case StmtStmtBase::Type::kWildSyn:
                return std::make_unique<T>(ArgPos::kSecond, second_syn_);
            case StmtStmtBase::Type::kWildWild:
                return std::make_unique<T>();
        }
    }
    template <typename T>
    std::unique_ptr<ConditionClause> BuildUsesModifiesClause() {
        static_assert(std::is_base_of_v<UsesModifiesBase, T>);
        UsesModifiesBase::Type type = UsesModifiesBase::UsesModifiesType(
                first_param_type_, second_param_type_);
        switch (type) {
            case UsesModifiesBase::Type::kIntSyn:
                return std::make_unique<T>(first_int_, second_syn_);
            case UsesModifiesBase::Type::kIntWild:
                return std::make_unique<T>(first_int_);
            case UsesModifiesBase::Type::kIntIdent:
                return std::make_unique<T>(first_int_, second_ident_);
            case UsesModifiesBase::Type::kSynSyn:
                return std::make_unique<T>(first_syn_, second_syn_);
            case UsesModifiesBase::Type::kSynWild:
                return std::make_unique<T>(first_syn_);
            case UsesModifiesBase::Type::kSynIdent:
                return std::make_unique<T>(first_syn_, second_ident_);
            case UsesModifiesBase::Type::kIdentSyn:
                return std::make_unique<T>(first_ident_, second_syn_);
            case UsesModifiesBase::Type::kIdentIdent:
                return std::make_unique<T>(first_ident_, second_ident_);
            case UsesModifiesBase::Type::kIdentWild:
                return std::make_unique<T>(second_ident_);
        }
    }
    template <typename T>
    std::unique_ptr<ConditionClause> BuildPatternClause() {
        static_assert(std::is_base_of_v<PatternBase, T>);
        PatternBase::Type type =
                PatternBase::PatternType(first_param_type_, second_param_type_);
        switch (type) {
            case PatternBase::Type::kWildWild:
                return std::make_unique<T>(assign_);
            case PatternBase::Type::kWildExpr:
                return std::make_unique<T>(assign_, std::move(second_exprs_));
            case PatternBase::Type::kIdentWild:
                return std::make_unique<T>(assign_, first_ident_);
            case PatternBase::Type::kIdentExpr:
                return std::make_unique<T>(assign_, first_ident_,
                                           std::move(second_exprs_));
            case PatternBase::Type::kSynWild:
                return std::make_unique<T>(assign_, first_syn_);
            case PatternBase::Type::kSynExpr:
                return std::make_unique<T>(assign_, first_syn_,
                                           std::move(second_exprs_));
        }
    }
    void Reset();
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_FACTORY_H_
