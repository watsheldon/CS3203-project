#ifndef SPA_SRC_SPA_SRC_QPS_GENERATOR_H_
#define SPA_SRC_SPA_SRC_QPS_GENERATOR_H_

#include <map>
#include <memory>
#include <string_view>
#include <vector>

#include "common/entity_type_enum.h"
#include "conditions/factory.h"
#include "query_object.h"
#include "query_token.h"
#include "synonym.h"
#include "synonym_with_attr.h"

namespace {
using namespace std::literals::string_view_literals;
}

namespace spa {
class Generator {
    using SynonymMap = std::map<std::string_view, Synonym* const>;
    using VecTokens = std::vector<QueryToken>;

  public:
    QueryObject Generate(const VecTokens& tokens) noexcept;

  private:
    enum class Mode {
        kDeclaration = 0,
        kMultiSelect,
        kSelect,
        kWith,
        kPattern,
        kFollows,
        kParent,
        kUses,
        kModifies,
        kCalls,
        kNext,
        kAffects,
        kExpression,
        kIdentifier,
        kZeroth,
        kFirst,
        kSecond,
        kThird,
        kNone
    };
    enum class ClauseMode {
        kSuchThat,
        kPattern,
        kWith,
    };
    static constexpr std::string_view kBoolean = "BOOLEAN"sv;
    static constexpr std::array<Synonym::Type, 3> kPatternSynonymType{
            Synonym::Type::kStmtIf, Synonym::Type::kStmtWhile,
            Synonym::Type::kStmtAssign};
    bool semantic_error_;
    bool can_terminate_;
    bool is_with_num_;
    bool is_left_zero_;
    SynonymMap synonym_map_;
    VecUniquePtr<Synonym> synonyms_;
    std::vector<SynonymWithAttr> selected_;
    std::vector<Mode> mode_;
    ClauseMode curr_clause_mode_;
    Synonym::Type curr_syn_type_;
    VecUniquePtr<ConditionClause> conditions_;
    std::vector<QueryToken> expression_;
    Synonym* pattern_syn_;
    Attribute first_attr_;
    Synonym* first_with_syn_;
    Synonym* second_with_syn_;
    Factory factory_;

    inline void SemanticError() noexcept;
    void BeginDecl(QueryTokenType token_type) noexcept;
    void BeginClause(QueryTokenType token_type) noexcept;
    void Asterisk() noexcept;
    void BracketL() noexcept;
    void BracketR() noexcept;
    void Name(const QueryToken& token) noexcept;
    void Constant(const QueryToken& token) noexcept;
    void AddDecl(std::string_view name) noexcept;
    void Select(std::string_view name) noexcept;
    void SetZeroth(std::string_view name) noexcept;
    void SetFirst(std::string_view name) noexcept;
    void SetSecond(std::string_view name) noexcept;
    void ParseToken(const QueryToken& token) noexcept;
    void Underscore() noexcept;
    void QuoteAsSecondArg() noexcept;
    void Quote() noexcept;
    void Comma() noexcept;
    void Semicolon() noexcept;
    void Attr(QueryTokenType token_type) noexcept;
    void AngleBracketL() noexcept;
    void AngleBracketR() noexcept;
    void SetClauseMode(QueryTokenType token_type) noexcept;
    void And() noexcept;
    void Equal() noexcept;
    void RightQuote() noexcept;
    void SetIsWithNum(Attribute attr) noexcept;
    void Identifier(const std::string& name) noexcept;

    static constexpr Synonym::Type TokenToSynType(QueryTokenType type) noexcept;
    static constexpr Mode TokenToClauseMode(QueryTokenType type) noexcept;
    static constexpr Attribute TokenToAttrType(QueryTokenType type) noexcept;
    static constexpr bool InvalidPatternSyn(Synonym::Type syn_type) noexcept;
    static constexpr bool UnsuitableFirstSynType(Mode mode,
                                                 Synonym::Type type) noexcept;
    static constexpr bool UnsuitableSecondSynType(Mode mode,
                                                  Synonym::Type type) noexcept;
    static constexpr bool UnsuitableAttributeType(Synonym* syn,
                                                  Attribute attr) noexcept;
    void Reset() noexcept;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_GENERATOR_H_
