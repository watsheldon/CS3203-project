#ifndef SRC_SPA_SRC_QPS_CONDITIONS_CALLBASE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_CALLBASE_H_

#include <array>
#include <string_view>
#include <variant>

#include "common/aliases.h"
#include "condition_clause.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"
#include "qps/synonym.h"

namespace spa {
class CallsBase : public ConditionClause {
  public:
    using Param = std::variant<Wildcard, Synonym *, ProcName>;
    enum Type {
        kSynSyn,
        kSynWild,
        kSynProc,
        kWildSyn,
        kWildWild,
        kWildProc,
        kProcSyn,
        kProcWild,
        kProcProc
    };
    static constexpr Type GetType(FirstParamType first,
                                  SecondParamType second) noexcept {
        int first_index = static_cast<int>(first) - 1;
        int second_index = second == SecondParamType::kIdent
                                   ? static_cast<int>(second) - 2
                                   : static_cast<int>(second) - 1;
        return kTypeMatrix[first_index][second_index];
    }
    CallsBase(Synonym *first, Synonym *second) noexcept;
    CallsBase(Synonym *syn, Wildcard) noexcept;
    CallsBase(Synonym *first, ProcName second) noexcept;
    CallsBase(Wildcard, Synonym *syn) noexcept;
    CallsBase(Wildcard, Wildcard) noexcept;
    CallsBase(Wildcard, ProcName second) noexcept;
    CallsBase(ProcName first, Synonym *second) noexcept;
    CallsBase(ProcName first, Wildcard) noexcept;
    CallsBase(ProcName first, ProcName second) noexcept;
    ResultTable Execute(KnowledgeBase *pkb) const noexcept final;

  protected:
    Type type_;
    Param first_param_;
    Param second_param_;
    static constexpr Array2D<Type, 3, 3> kTypeMatrix{
            {{Type::kSynSyn, Type::kSynWild, Type::kSynProc},
             {Type::kWildSyn, Type::kWildWild, Type::kWildProc},
             {Type::kProcSyn, Type::kProcWild, Type::kProcProc}}};
    virtual ResultTable SynSyn(KnowledgeBase *pkb, Synonym *first,
                               Synonym *second) const noexcept = 0;
    virtual ResultTable SynProc(KnowledgeBase *pkb, Synonym *first,
                                ProcName second) const noexcept = 0;
    virtual ResultTable ProcSyn(KnowledgeBase *pkb, ProcName first,
                                Synonym *second) const noexcept = 0;
    virtual ResultTable ProcProc(KnowledgeBase *pkb, ProcName first,
                                 ProcName second) const noexcept = 0;
    [[nodiscard]] int GetSynCount() const noexcept final;

  private:
    static constexpr std::array<int, 9> kSynCount{2, 1, 1, 1, 0, 0, 1, 0, 0};
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_CALLBASE_H_
