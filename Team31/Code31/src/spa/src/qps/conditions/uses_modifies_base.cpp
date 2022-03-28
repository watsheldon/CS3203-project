#include "uses_modifies_base.h"

#include "qps/synonym.h"

namespace spa {
UsesModifiesBase::UsesModifiesBase(int first, VarName second) noexcept
        : type_(Type::kNumVar), first_param_(first), second_param_(second) {}
UsesModifiesBase::UsesModifiesBase(int first, Synonym *second) noexcept
        : type_(Type::kNumSyn), first_param_(first), second_param_(second) {}
UsesModifiesBase::UsesModifiesBase(int first) noexcept
        : type_(Type::kNumWild), first_param_(first) {}
UsesModifiesBase::UsesModifiesBase(Synonym *first, VarName second) noexcept
        : type_(Type::kSynVar), first_param_(first), second_param_(second) {}
UsesModifiesBase::UsesModifiesBase(Synonym *first, Synonym *second) noexcept
        : type_(Type::kSynSyn), first_param_(first), second_param_(second) {}
UsesModifiesBase::UsesModifiesBase(Synonym *first) noexcept
        : type_(Type::kSynWild), first_param_(first) {}
UsesModifiesBase::UsesModifiesBase(ProcName first, Synonym *second) noexcept
        : type_(Type::kProcSyn), first_param_(first), second_param_(second) {}
UsesModifiesBase::UsesModifiesBase(ProcName first, VarName second) noexcept
        : type_(Type::kProcVar), first_param_(first), second_param_(second) {}
UsesModifiesBase::UsesModifiesBase(ProcName first) noexcept
        : type_(Type::kProcWild), first_param_(first) {}
ResultTable UsesModifiesBase::Execute(KnowledgeBase *pkb) const noexcept {
    switch (type_) {
        case kNumVar: {
            auto first_stmt_no = std::get<StmtNo>(first_param_);
            auto second_var_name = std::get<VarName>(second_param_);
            return NumVar(pkb, first_stmt_no, second_var_name);
        }
        case kNumSyn: {
            auto first_stmt_no = std::get<StmtNo>(first_param_);
            auto second_syn = std::get<Synonym *>(second_param_);
            second_syn->IncResolved();
            return NumSyn(pkb, first_stmt_no, second_syn);
        }
        case kNumWild: {
            auto first_stmt_no = std::get<StmtNo>(first_param_);
            return NumWild(pkb, first_stmt_no);
        }
        case kSynVar: {
            auto first_syn = std::get<Synonym *>(first_param_);
            auto second_var_name = std::get<VarName>(second_param_);
            first_syn->IncResolved();
            return SynVar(pkb, first_syn, second_var_name);
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
        case kProcVar: {
            auto first_proc_name = std::get<ProcName>(first_param_);
            auto second_var_name = std::get<VarName>(second_param_);
            return ProcVar(pkb, first_proc_name, second_var_name);
        }
        case kProcSyn: {
            auto first_proc_name = std::get<ProcName>(first_param_);
            auto second_syn = std::get<Synonym *>(second_param_);
            second_syn->IncResolved();
            return ProcSyn(pkb, first_proc_name, second_syn);
        }
        case kProcWild: {
            auto first_proc_name = std::get<ProcName>(first_param_);
            return ProcWild(pkb, first_proc_name);
        }
    }
    assert(false);
    return ResultTable{false};
}
}  // namespace spa
