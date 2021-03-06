#ifndef SRC_SPA_SRC_QPS_CONDITIONS_STMTSTMTBASE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_STMTSTMTBASE_H_

#include <array>
#include <string_view>
#include <variant>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "condition_clause.h"
#include "qps/synonym.h"

namespace spa {
class StmtStmtBase : public ConditionClause {
  public:
    using Param = std::variant<Wildcard, StmtNo, Synonym *>;
    /**
     * This is seemingly a violation of OCP, where exhaustive types are
     * declared, making it appear to be closed for extension. However, condition
     * clauses in the current design are meant to function mostly as a data
     * structure, albeit with a bit of logic just to facilitate PKB's job. Most
     * of the business logic to find the information is located inside the PKB,
     * extensibility is trivial as more types can easily be added, with a
     * corresponding method call in the PKB itself. With the design of the PKB,
     * it is unlikely that the condition clauses can be designed in a way
     * that's more typical of an implementation that relies on the QPS to do the
     * calculations.
     *
     * Admittedly it is possible to avoid the exhaustive type declaration, by
     * inferring from the types of the arguments at run time. However, that is
     * throwing away information that is obtained during the parsing process,
     * introducing unnecessary overhead to recover the information, while making
     * the code violate basic OOP principles by having to check the actual type
     * within the sum types.
     */
    enum Type {
        kNumNum,
        kNumSyn,
        kNumWild,
        kSynNum,
        kSynSyn,
        kSynWild,
        kWildNum,
        kWildSyn,
        kWildWild,
    };
    StmtStmtBase() noexcept;
    StmtStmtBase(StmtNo first, StmtNo second) noexcept;
    StmtStmtBase(StmtNo first, Synonym *second) noexcept;
    StmtStmtBase(Synonym *first, StmtNo second) noexcept;
    StmtStmtBase(Synonym *first, Synonym *second) noexcept;
    StmtStmtBase(Wildcard, StmtNo stmt_no) noexcept;
    StmtStmtBase(StmtNo stmt_no, Wildcard) noexcept;
    StmtStmtBase(Wildcard, Synonym *syn) noexcept;
    StmtStmtBase(Synonym *syn, Wildcard) noexcept;
    static constexpr Type GetType(FirstParamType first,
                                  SecondParamType second) noexcept {
        int first_index = static_cast<int>(first);
        int second_index = static_cast<int>(second);
        return kTypeMatrix[first_index][second_index];
    }
    ~StmtStmtBase() override = default;
    ResultTable Execute(KnowledgeBase *pkb) const noexcept final;

  protected:
    Type type_;
    Param first_param_;
    Param second_param_;
    static constexpr Array2D<Type, 3, 3> kTypeMatrix{
            {{Type::kNumNum, Type::kNumSyn, Type::kNumWild},
             {Type::kSynNum, Type::kSynSyn, Type::kSynWild},
             {Type::kWildNum, Type::kWildSyn, Type::kWildWild}}};
    virtual ResultTable NumNum(KnowledgeBase *pkb, StmtNo first,
                               StmtNo second) const noexcept = 0;
    virtual ResultTable NumSyn(KnowledgeBase *pkb, StmtNo first,
                               Synonym *second) const noexcept = 0;
    virtual ResultTable NumWild(KnowledgeBase *pkb,
                                StmtNo first) const noexcept = 0;
    virtual ResultTable SynNum(KnowledgeBase *pkb, Synonym *first,
                               StmtNo second) const noexcept = 0;
    virtual ResultTable SynSyn(KnowledgeBase *pkb, Synonym *first,
                               Synonym *second) const noexcept = 0;
    virtual ResultTable SynWild(KnowledgeBase *pkb,
                                Synonym *first) const noexcept = 0;
    virtual ResultTable WildNum(KnowledgeBase *pkb,
                                StmtNo second) const noexcept = 0;
    virtual ResultTable WildSyn(KnowledgeBase *pkb,
                                Synonym *second) const noexcept = 0;
    virtual ResultTable WildWild(KnowledgeBase *pkb) const noexcept = 0;
    [[nodiscard]] int GetSynCount() const noexcept final;

  private:
    static constexpr std::array<int, 9> kSynCount{0, 1, 0, 1, 2, 1, 0, 1, 0};
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_CONDITIONS_STMTSTMTBASE_H_
