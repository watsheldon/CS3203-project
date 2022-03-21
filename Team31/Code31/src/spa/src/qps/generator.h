#ifndef SPA_SRC_SPA_SRC_QPS_GENERATOR_H_
#define SPA_SRC_SPA_SRC_QPS_GENERATOR_H_

#include <map>
#include <memory>
#include <string_view>
#include <vector>

#include "conditions/factory.h"
#include "query_object.h"
#include "query_token.h"
#include "synonym.h"

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
        kSelect,
        kParent,
        kFollows,
        kUses,
        kModifies,
        kCalls,
        kPattern,
        kExpression,
        kIdentifier,
        kZeroth,
        kFirst,
        kSecond,
    };
    static constexpr std::string_view kBoolean = "BOOLEAN"sv;
    bool semantic_error_;
    SynonymMap synonym_map_;
    VecUniquePtr<Synonym> synonyms_;
    std::vector<const Synonym*> selected_;
    std::vector<Mode> mode_;
    Synonym::Type curr_syn_type_;
    VecUniquePtr<ConditionClause> conditions_;
    std::vector<QueryToken> expression_;
    Factory factory_;

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
    void Quote() noexcept;
    void Comma() noexcept;
    void Semicolon() noexcept;

    static constexpr Synonym::Type TokenToSynType(QueryTokenType type) noexcept;
    static constexpr Mode TokenToClauseMode(QueryTokenType type) noexcept;
    static constexpr bool UnsuitableFirstSynType(Mode mode,
                                                 Synonym::Type type) noexcept;
    static constexpr bool UnsuitableSecondSynType(Mode mode,
                                                  Synonym::Type type) noexcept;
    void Reset() noexcept;
};
}  // namespace spa
#endif  // SPA_SRC_SPA_SRC_QPS_GENERATOR_H_
