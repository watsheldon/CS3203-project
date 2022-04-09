#include "calls_base.h"

#include "qps/synonym.h"

namespace spa {
CallsBase::CallsBase(Synonym *first, Synonym *second) noexcept
        : type_(kSynSyn), first_param_(first), second_param_(second) {}
CallsBase::CallsBase(Synonym *syn, Wildcard) noexcept
        : type_(kSynWild), first_param_(syn) {}
CallsBase::CallsBase(Synonym *first, ProcName second) noexcept
        : type_(kSynProc), first_param_(first), second_param_(second) {}
CallsBase::CallsBase(Wildcard, Synonym *syn) noexcept
        : type_(kWildSyn), second_param_(syn) {}
CallsBase::CallsBase(Wildcard, Wildcard) noexcept : type_(kWildWild) {}
CallsBase::CallsBase(Wildcard, ProcName second) noexcept
        : type_(kWildProc), second_param_(second) {}
CallsBase::CallsBase(ProcName first, Synonym *second) noexcept
        : type_(kProcSyn), first_param_(first), second_param_(second) {}
CallsBase::CallsBase(ProcName first, Wildcard) noexcept
        : type_(kProcWild), first_param_(first) {}
CallsBase::CallsBase(ProcName first, ProcName second) noexcept
        : type_(kProcProc), first_param_(first), second_param_(second) {}
ResultTable CallsBase::Execute(KnowledgeBase *pkb) const noexcept {
    switch (type_) {
        case kSynSyn: {
            auto first_syn = std::get<Synonym *>(first_param_);
            auto second_syn = std::get<Synonym *>(second_param_);
            first_syn->IncResolved(), second_syn->IncResolved();
            return SynSyn(pkb, first_syn, second_syn);
        }
        case kSynWild: {
            auto first_syn = std::get<Synonym *>(first_param_);
            first_syn->IncResolved();
            auto result = pkb->GetCalls(ArgPos::kFirst);
            return {first_syn, std::move(result)};
        }
        case kSynProc: {
            auto first_syn = std::get<Synonym *>(first_param_);
            auto second_proc = std::get<ProcName>(second_param_);
            first_syn->IncResolved();
            return SynProc(pkb, first_syn, second_proc);
        }
        case kWildSyn: {
            auto second_syn = std::get<Synonym *>(second_param_);
            second_syn->IncResolved();
            auto result = pkb->GetCalls(ArgPos::kSecond);
            return {second_syn, std::move(result)};
        }
        case kWildWild:
            return ResultTable{pkb->ExistCalls()};
        case kWildProc: {
            auto second_proc = std::get<ProcName>(second_param_);
            bool result = pkb->ExistCalls(Name<ArgPos::kSecond>(second_proc));
            return ResultTable(result);
        }
        case kProcSyn: {
            auto first_proc = std::get<ProcName>(first_param_);
            auto second_syn = std::get<Synonym *>(second_param_);
            second_syn->IncResolved();
            return ProcSyn(pkb, first_proc, second_syn);
        }
        case kProcWild: {
            auto first_proc = std::get<ProcName>(first_param_);
            auto result = pkb->ExistCalls(Name<ArgPos::kFirst>(first_proc));
            return ResultTable(result);
        }
        case kProcProc: {
            auto first_proc = std::get<ProcName>(first_param_);
            auto second_proc = std::get<ProcName>(second_param_);
            return ProcProc(pkb, first_proc, second_proc);
        }
    }
    assert(false);
    return ResultTable{false};
}
int CallsBase::GetSynCount() const noexcept {
    return kSynCount[static_cast<int>(type_)];
}
}  // namespace spa
