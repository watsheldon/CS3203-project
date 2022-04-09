#ifndef SRC_SPA_SRC_QPS_CONDITIONS_USESMODIFIESBASE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_USESMODIFIESBASE_H_

#include <array>
#include <string>
#include <string_view>
#include <variant>

#include "common/aliases.h"
#include "condition_clause.h"
#include "pkb/knowledge_base.h"
#include "qps/evaluator/result_table.h"
#include "qps/synonym.h"

namespace spa {
class UsesModifiesBase : public ConditionClause {
  public:
    using FirstParam = std::variant<StmtNo, Synonym *, ProcName>;
    using SecondParam = std::variant<Wildcard, Synonym *, VarName>;
    enum Type {
        kNumVar,
        kNumSyn,
        kNumWild,
        kSynVar,
        kSynSyn,
        kSynWild,
        kProcVar,
        kProcSyn,
        kProcWild
    };

    UsesModifiesBase(StmtNo first, VarName second) noexcept;
    UsesModifiesBase(StmtNo first, Synonym *second) noexcept;
    explicit UsesModifiesBase(StmtNo first) noexcept;
    UsesModifiesBase(Synonym *first, VarName second) noexcept;
    UsesModifiesBase(Synonym *first, Synonym *second) noexcept;
    explicit UsesModifiesBase(Synonym *first) noexcept;
    UsesModifiesBase(ProcName first, Synonym *second) noexcept;
    UsesModifiesBase(ProcName first, VarName second) noexcept;
    explicit UsesModifiesBase(ProcName first) noexcept;
    static constexpr Type GetType(FirstParamType first,
                                  SecondParamType second) noexcept {
        int first_index = first == FirstParamType::kIdent
                                  ? static_cast<int>(first) - 1
                                  : static_cast<int>(first);
        int second_index = second == SecondParamType::kIdent
                                   ? static_cast<int>(second) - 2
                                   : static_cast<int>(second) - 1;
        return kTypeMatrix[first_index][second_index];
    }
    ~UsesModifiesBase() override = default;

  protected:
    Type type_;
    FirstParam first_param_;
    SecondParam second_param_;
    static constexpr Array2D<Type, 3, 3> kTypeMatrix{
            {{Type::kNumSyn, Type::kNumWild, Type::kNumVar},
             {Type::kSynSyn, Type::kSynWild, Type::kSynVar},
             {Type::kProcSyn, Type::kProcWild, Type::kProcVar}}};

    ResultTable Execute(KnowledgeBase *pkb) const noexcept final;
    virtual ResultTable NumVar(KnowledgeBase *pkb, StmtNo first,
                               VarName second) const noexcept = 0;
    virtual ResultTable NumSyn(KnowledgeBase *pkb, StmtNo first,
                               Synonym *second) const noexcept = 0;
    virtual ResultTable NumWild(KnowledgeBase *pkb,
                                StmtNo first) const noexcept = 0;
    virtual ResultTable SynVar(KnowledgeBase *pkb, Synonym *first,
                               VarName second) const noexcept = 0;
    virtual ResultTable SynSyn(KnowledgeBase *pkb, Synonym *first,
                               Synonym *second) const noexcept = 0;
    virtual ResultTable SynWild(KnowledgeBase *pkb,
                                Synonym *first) const noexcept = 0;
    virtual ResultTable ProcVar(KnowledgeBase *pkb, ProcName first,
                                VarName second) const noexcept = 0;
    virtual ResultTable ProcSyn(KnowledgeBase *pkb, ProcName first,
                                Synonym *second) const noexcept = 0;
    virtual ResultTable ProcWild(KnowledgeBase *pkb,
                                 ProcName first) const noexcept = 0;
    [[nodiscard]] int GetSynCount() const noexcept override;

  private:
    static constexpr std::array<int, 9> kSynCount{0, 1, 0, 1, 2, 1, 0, 1, 0};
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_USESMODIFIESBASE_H_
