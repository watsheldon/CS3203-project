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
        kNone,
        kFollows,
        kFollowsTrans,
        kParent,
        kParentTrans,
        kUses,
        kModifies,
        kPatternPartial,
        kPatternExact,
        kNext,
        kNextTrans,
        kAffects,
        kAffectsTrans,
        kCalls
    };
    enum ParamType {
        kNoneNone,
        kIntInt,
        kIntSyn,
        kIntWild,
        kIntIdent,
        kSynInt,
        kSynSyn,
        kSynWild,
        kSynIdent,
        kSynExpr,
        kWildInt,
        kWildSyn,
        kWildWild,
        kWildExpr,
        kIdentSyn,
        kIdentIdent,
        kIdentWild,
        kIdentExpr,
    };
    Relationship rel_;
    ParamType param_type_;
    int first_int_;
    int second_int_;
    std::string first_ident_;
    std::string second_ident_;
    Synonym* assign_;
    Synonym* first_syn_;
    Synonym* second_syn_;
    std::vector<QueryToken> second_exprs_;
    int first_index_ = 0;
    int second_index_ = 0;
    static constexpr std::array<std::array<ParamType, 3>, 3> stmt_stmt_type_ = {
            {{
                     ParamType::kWildWild,
                     ParamType::kWildInt,
                     ParamType::kWildSyn,
             },
             {
                     ParamType::kIntWild,
                     ParamType::kIntInt,
                     ParamType::kIntSyn,
             },
             {
                     ParamType::kSynWild,
                     ParamType::kSynInt,
                     ParamType::kSynSyn,
             }}};
    static constexpr std::array<std::array<ParamType, 3>, 3>
            uses_modifies_type_ = {{
                    {
                            ParamType::kIdentWild,
                            ParamType::kIdentIdent,
                            ParamType::kIdentSyn,
                    },
                    {ParamType::kIntWild, ParamType::kIntIdent,
                     ParamType::kIntSyn

                    },
                    {ParamType::kSynWild, ParamType::kSynIdent,
                     ParamType::kSynSyn},
            }};
    static constexpr std::array<std::array<ParamType, 2>, 3> pattern_type_ = {{
            {ParamType::kWildWild, ParamType::kWildExpr},

            {ParamType::kIdentWild, ParamType::kIdentExpr

            },
            {ParamType::kSynWild, ParamType::kSynExpr

            },
    }};
    template <typename T>
    std::unique_ptr<ConditionClause> BuildStmtStmtClause() {
        static_assert(std::is_base_of_v<StmtStmtBase, T>);
        switch (param_type_) {
            case kIntInt:
                return std::make_unique<T>(first_int_, second_int_);
            case kIntSyn:
                return std::make_unique<T>(first_int_, second_syn_);
            case kIntWild:
                return std::make_unique<T>(ArgPos::kFirst, first_int_);
            case kIntIdent:
                return std::make_unique<T>(first_int_, second_syn_);
            case kSynInt:
                return std::make_unique<T>(first_syn_, second_int_);
            case kSynSyn:
                if constexpr (std::is_same_v<T, FollowsClause> ||
                              std::is_same_v<T, FollowsTransClause> ||
                              std::is_same_v<T, ParentClause> ||
                              std::is_same_v<T, ParentTransClause>) {
                    if (first_syn_ == second_syn_) {
                        return nullptr;
                    }
                }
                return std::make_unique<T>(first_syn_, second_syn_);
            case kSynWild:
                return std::make_unique<T>(ArgPos::kFirst, first_syn_);
            case kWildInt:
                return std::make_unique<T>(ArgPos::kSecond, second_int_);
            case kWildSyn:
                return std::make_unique<T>(ArgPos::kSecond, second_syn_);
            case kWildWild:
                return std::make_unique<T>();
            default:
                assert(false);
        }
    }
    template <typename T>
    std::unique_ptr<ConditionClause> BuildUsesModifiesClause() {
        static_assert(std::is_base_of_v<UsesModifiesBase, T>);
        switch (param_type_) {
            case kIntSyn:
                return std::make_unique<T>(first_int_, second_syn_);
            case kIntWild:
                return std::make_unique<T>(first_int_);
            case kIntIdent:
                return std::make_unique<T>(first_int_, second_ident_);
            case kSynSyn:
                return std::make_unique<T>(first_syn_, second_syn_);
            case kSynWild:
                return std::make_unique<T>(first_syn_);
            case kSynIdent:
                return std::make_unique<T>(first_syn_, second_ident_);
            case kIdentSyn:
                return std::make_unique<T>(first_ident_, second_syn_);
            case kIdentIdent:
                return std::make_unique<T>(first_ident_, second_ident_);
            case kIdentWild:
                return std::make_unique<T>(second_ident_);
            default:
                assert(false);
        }
    }
    template <typename T>
    std::unique_ptr<ConditionClause> BuildPatternClause() {
        static_assert(std::is_base_of_v<PatternBase, T>);
        switch (param_type_) {
            case kWildWild:
                return std::make_unique<T>(assign_);
            case kWildExpr:
                return std::make_unique<T>(assign_, std::move(second_exprs_));
            case kIdentWild:
                return std::make_unique<T>(assign_, first_ident_);
            case kIdentExpr:
                return std::make_unique<T>(assign_, first_ident_,
                                           std::move(second_exprs_));
            case kSynWild:
                return std::make_unique<T>(assign_, first_syn_);
            case kSynExpr:
                return std::make_unique<T>(assign_, first_syn_,
                                           std::move(second_exprs_));
            default:
                assert(false);
        }
    }
    void Reset();
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_FACTORY_H_
