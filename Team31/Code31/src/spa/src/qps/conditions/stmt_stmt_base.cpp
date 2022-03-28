#include "stmt_stmt_base.h"

#include "qps/synonym.h"

namespace spa {
StmtStmtBase::StmtStmtBase() noexcept : type_(Type::kWildWild) {}
StmtStmtBase::StmtStmtBase(int first, int second) noexcept
        : type_(Type::kNumNum), first_param_(first), second_param_(second) {}
StmtStmtBase::StmtStmtBase(int first, Synonym *second) noexcept
        : type_(Type::kNumSyn), first_param_(first), second_param_(second) {}
StmtStmtBase::StmtStmtBase(Synonym *first, int second) noexcept
        : type_(Type::kSynNum), first_param_(first), second_param_(second) {}
StmtStmtBase::StmtStmtBase(Synonym *first, Synonym *second) noexcept
        : type_(Type::kSynSyn), first_param_(first), second_param_(second) {}
StmtStmtBase::StmtStmtBase(ConditionClause::Wildcard, StmtNo stmt_no) noexcept
        : type_(kWildNum), second_param_(stmt_no) {}
StmtStmtBase::StmtStmtBase(StmtNo stmt_no, ConditionClause::Wildcard) noexcept
        : type_(kNumWild), first_param_(stmt_no) {}
StmtStmtBase::StmtStmtBase(Wildcard, Synonym *syn) noexcept
        : type_(kWildSyn), second_param_(syn) {}
StmtStmtBase::StmtStmtBase(Synonym *syn, Wildcard) noexcept
        : type_(kSynWild), first_param_(syn) {}
ResultTable StmtStmtBase::Execute(KnowledgeBase *pkb) const noexcept {
    switch (type_) {
        case kNumNum: {
            const auto first_stmt_no = std::get<StmtNo>(first_param_);
            const auto second_stmt_no = std::get<StmtNo>(second_param_);
            return NumNum(pkb, first_stmt_no, second_stmt_no);
        }
        case kNumSyn: {
            const auto first_stmt_no = std::get<StmtNo>(first_param_);
            auto second_syn = std::get<Synonym *>(second_param_);
            second_syn->IncResolved();
            return NumSyn(pkb, first_stmt_no, second_syn);
        }
        case kNumWild: {
            const auto first_stmt_no = std::get<StmtNo>(first_param_);
            return NumWild(pkb, first_stmt_no);
        }
        case kSynNum: {
            auto first_syn = std::get<Synonym *>(first_param_);
            const auto second_stmt_no = std::get<StmtNo>(second_param_);
            first_syn->IncResolved();
            return SynNum(pkb, first_syn, second_stmt_no);
        }
        case kSynSyn: {
            auto first_syn = std::get<Synonym *>(first_param_);
            auto second_syn = std::get<Synonym *>(second_param_);
            first_syn->IncResolved(), second_syn->IncResolved();
            return SynSyn(pkb, first_syn, second_syn);
        }
        case kSynWild: {
            auto first_syn = std::get<Synonym *>(first_param_);
            first_syn->IncResolved();
            return SynWild(pkb, first_syn);
        }
        case kWildNum: {
            auto second_stmt_no = std::get<StmtNo>(second_param_);
            return WildNum(pkb, second_stmt_no);
        }
        case kWildSyn: {
            auto second_syn = std::get<Synonym *>(second_param_);
            second_syn->IncResolved();
            return WildSyn(pkb, second_syn);
        }
        case kWildWild: {
            return WildWild(pkb);
        }
    }
    assert(false);
    return ResultTable{false};
}
}  // namespace spa
