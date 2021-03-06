#ifndef SRC_SPA_SRC_QPS_CONDITIONS_WITH_CLAUSE_H_
#define SRC_SPA_SRC_QPS_CONDITIONS_WITH_CLAUSE_H_

#include <array>
#include <string_view>
#include <variant>

#include "common/aliases.h"
#include "common/entity_type_enum.h"
#include "condition_clause.h"
#include "qps/synonym_with_attr.h"

namespace spa {
class WithClause : public ConditionClause {
  public:
    using Param = std::variant<std::string_view, SynonymWithAttr>;
    enum Type {
        kRawRaw,
        kRawSyn,
        kSynSyn,
    };
    WithClause(std::string_view first_ident,
               std::string_view second_ident) noexcept;
    WithClause(std::string_view string_view, SynonymWithAttr syn) noexcept;
    WithClause(SynonymWithAttr syn, std::string_view string_view) noexcept;
    WithClause(SynonymWithAttr first_syn, SynonymWithAttr second_syn) noexcept;
    ~WithClause() override = default;
    ResultTable Execute(KnowledgeBase *pkb) const noexcept final;

  protected:
    [[nodiscard]] int GetPriority() const noexcept final;
    [[nodiscard]] int GetSynCount() const noexcept final;

  private:
    Type type_;
    Param first_param_;
    Param second_param_;
    [[nodiscard]] ResultTable RawRaw() const noexcept;
    static ResultTable NameSyn(KnowledgeBase *pkb, std::string_view first,
                               SynonymWithAttr second) noexcept;
    static ResultTable StmtSyn(KnowledgeBase *pkb, std::string_view value,
                               SynonymWithAttr second) noexcept;
    ResultTable SynSyn(KnowledgeBase *pkb) const noexcept;
    ResultTable SynSynName(KnowledgeBase *pkb) const noexcept;
    ResultTable SynSynNum(KnowledgeBase *pkb) const noexcept;
    static ResultTable ValueValue(KnowledgeBase *pkb, SynonymWithAttr value_1,
                                  SynonymWithAttr value_2) noexcept;
    static ResultTable ValueStmt(KnowledgeBase *pkb, SynonymWithAttr value,
                                 SynonymWithAttr stmt) noexcept;
    static ResultTable StmtStmt(KnowledgeBase *pkb, SynonymWithAttr stmt_1,
                                SynonymWithAttr stmt_2) noexcept;
    static PairVec<int, std::string_view> ToNames(KnowledgeBase *pkb,
                                                  SynonymWithAttr syn) noexcept;
    static constexpr std::array<int, 3> kSynCount{0, 1, 2};
    static constexpr std::array<int, 3> kPriority{4, 4, 11};
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_CONDITIONS_WITH_CLAUSE_H_
