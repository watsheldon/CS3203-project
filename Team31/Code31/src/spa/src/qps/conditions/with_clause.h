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
    Type type_;
    Param first_param_;
    Param second_param_;
    static ResultTable RawRaw(KnowledgeBase *pkb, Param first,
                              Param second) noexcept;
    static ResultTable NameSyn(KnowledgeBase *pkb, std::string_view first,
                               SynonymWithAttr second) noexcept;
    static ResultTable IntSyn(KnowledgeBase *pkb, std::string_view value,
                              SynonymWithAttr second) noexcept;
    static ResultTable SynSyn(KnowledgeBase *pkb, SynonymWithAttr first,
                              SynonymWithAttr second) noexcept;
    static ResultTable SynSynNum(KnowledgeBase *pkb, SynonymWithAttr first,
                                 SynonymWithAttr second) noexcept;
    static ResultTable ValueStmt(KnowledgeBase *pkb, SynonymWithAttr first,
                                 SynonymWithAttr second) noexcept;
    [[nodiscard]] int GetPriority() const noexcept final;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_QPS_CONDITIONS_WITH_CLAUSE_H_
