#include "generator.h"

#include <cassert>
#include <memory>

#include "conditions/condition_clause.h"

namespace spa {
QueryObject Generator::Generate(const VecTokens &tokens) noexcept {
    Reset();
    for (const auto &token : tokens) {
        ParseToken(token);
        if (semantic_error_) return QueryObject(std::move(selected_));
    }
    return {std::move(selected_), std::move(synonyms_), std::move(conditions_)};
}

constexpr Synonym::Type Generator::TokenToSynType(
        QueryTokenType type) noexcept {
    switch (type) {
        case QueryTokenType::kDeclStmt:
            return Synonym::kStmtAny;
        case QueryTokenType::kDeclRead:
            return Synonym::kStmtRead;
        case QueryTokenType::kDeclPrint:
            return Synonym::kStmtPrint;
        case QueryTokenType::kDeclCall:
            return Synonym::kStmtCall;
        case QueryTokenType::kDeclAssign:
            return Synonym::kStmtAssign;
        case QueryTokenType::kDeclIf:
            return Synonym::kStmtIf;
        case QueryTokenType::kDeclWhile:
            return Synonym::kStmtWhile;
        case QueryTokenType::kDeclVariable:
            return Synonym::kVar;
        case QueryTokenType::kDeclConstant:
            return Synonym::kConst;
        case QueryTokenType::kDeclProcedure:
            return Synonym::kProc;
        default:
            assert(false);
    }
}
constexpr Generator::Mode Generator::TokenToClauseMode(
        QueryTokenType type) noexcept {
    switch (type) {
        case QueryTokenType::kKeywordParent:
            return Mode::kParent;
        case QueryTokenType::kKeywordFollows:
            return Mode::kFollows;
        case QueryTokenType::kKeywordUses:
            return Mode::kUses;
        case QueryTokenType::kKeywordModifies:
            return Mode::kModifies;
        case QueryTokenType::kKeywordPattern:
            return Mode::kPattern;
        case QueryTokenType::kKeywordCalls:
            return Mode::kCalls;
        default:
            assert(false);
    }
}
constexpr bool Generator::UnsuitableFirstSynType(Generator::Mode mode,
                                                 Synonym::Type type) noexcept {
    assert(type != Synonym::kNone);
    switch (mode) {
        case Mode::kParent:
            return type != Synonym::kStmtIf && type != Synonym::kStmtWhile &&
                   type != Synonym::kStmtAny;
        case Mode::kFollows:
            return type > Synonym::kStmtAssign;
        case Mode::kUses:
            return type > Synonym::kProc || type == Synonym::kStmtRead;
        case Mode::kModifies:
            return type > Synonym::kProc || type == Synonym::kStmtPrint;
        case Mode::kPattern:
            return type != Synonym::kVar;
        case Mode::kCalls:
            return type != Synonym::kProc;
        default:
            assert(false);
    }
}
constexpr bool Generator::UnsuitableSecondSynType(Generator::Mode mode,
                                                  Synonym::Type type) noexcept {
    assert(type != Synonym::kNone);
    switch (mode) {
        case Mode::kParent:
        case Mode::kFollows:
            return type > Synonym::kStmtAssign;
        case Mode::kUses:
        case Mode::kModifies:
            return type != Synonym::kVar;
        case Mode::kCalls:
            return type != Synonym::kProc;
        default:
            assert(false);
    }
}
void Generator::Reset() noexcept {
    semantic_error_ = false;
    synonym_map_.clear();
    synonyms_.clear();
    selected_.clear();
    mode_.clear();
    curr_syn_type_ = Synonym::kNone;
    conditions_.clear();
    expression_.clear();
}
void Generator::BeginDecl(QueryTokenType token_type) noexcept {
    mode_.emplace_back(Mode::kDeclaration);
    curr_syn_type_ = TokenToSynType(token_type);
}
void Generator::BeginClause(QueryTokenType token_type) noexcept {
    factory_.SetRelationship(token_type);
    mode_.emplace_back(TokenToClauseMode(token_type));
    mode_.emplace_back(Mode::kZeroth);
}
void Generator::Asterisk() noexcept {
    if (mode_.back() == Mode::kZeroth) {
        return factory_.SetTransPartial();
    }
    assert(mode_.back() == Mode::kExpression);
    expression_.emplace_back(QueryTokenType::kOperatorTimes);
}
void Generator::BracketL() noexcept {
    if (mode_.back() == Mode::kZeroth) {
        mode_.back() = Mode::kFirst;
        return;
    }
    assert(mode_.back() == Mode::kExpression);
    expression_.emplace_back(QueryTokenType::kBracketL);
}
void Generator::BracketR() noexcept {
    if (mode_.back() == Mode::kExpression) {
        expression_.emplace_back(QueryTokenType::kBracketR);
    } else if (mode_.back() == Mode::kSecond) {
        mode_.pop_back();
        return BracketR();
    }
    assert(mode_.back() >= Mode::kParent && mode_.back() <= Mode::kPattern);
    mode_.pop_back();
    auto clause = factory_.Build();
    if (clause == nullptr) {
        semantic_error_ = true;
        return;
    }
    conditions_.emplace_back(std::move(clause));
}
void Generator::Name(const QueryToken &token) noexcept {
    std::string_view name{token.value};
    switch (mode_.back()) {
        case Mode::kDeclaration:
            return AddDecl(name);
        case Mode::kSelect:
            return Select(name);
        case Mode::kExpression:
            expression_.emplace_back(token);
            return;
        case Mode::kIdentifier:
            mode_.pop_back();
            mode_.back() == Mode::kFirst ? factory_.SetFirst(token.value)
                                         : factory_.SetSecond(token.value);
            mode_.emplace_back(Mode::kIdentifier);
            return;
        case Mode::kZeroth:
            return SetZeroth(name);
        case Mode::kFirst:
            return SetFirst(name);
        case Mode::kSecond:
            return SetSecond(name);
        default:
            assert(false);
    }
}
void Generator::Constant(const QueryToken &token) noexcept {
    const auto &val = token.value;
    if (mode_.back() == Mode::kExpression) {
        expression_.emplace_back(token);
        return;
    }
    // stmt#
    auto value = std::stoi(val);
    if (value == 0) {
        semantic_error_ = true;
        return;
    }
    mode_.back() == Mode::kFirst ? factory_.SetFirst(value)
                                 : factory_.SetSecond(value);
    mode_.pop_back();
}
void Generator::AddDecl(std::string_view name) noexcept {
    auto &ptr =
            synonyms_.emplace_back(std::make_unique<Synonym>(curr_syn_type_));
    auto [itr, inserted] = synonym_map_.try_emplace(name, ptr.get());
    if (!inserted) {
        semantic_error_ = true;
        return;
    }
}
void Generator::Select(std::string_view name) noexcept {
    auto itr = synonym_map_.find(name);
    if (itr == synonym_map_.end()) {
        if (name == kBoolean) {
            selected_.emplace_back(nullptr);
            return;
        }
        semantic_error_ = true;
        return;
    }
    selected_.emplace_back(itr->second);
    mode_.pop_back();
}
void Generator::SetZeroth(std::string_view name) noexcept {
    auto itr = synonym_map_.find(name);
    if (itr == synonym_map_.end() ||
        itr->second->type != Synonym::kStmtAssign) {
        semantic_error_ = true;
        return;
    }
    auto syn = itr->second;
    syn->IncRef();
    factory_.SetAssign(syn);
}
void Generator::SetFirst(std::string_view name) noexcept {
    mode_.pop_back();
    auto itr = synonym_map_.find(name);
    if (itr == synonym_map_.end()) {
        semantic_error_ = true;
        return;
    }
    auto syn = itr->second;
    if (UnsuitableFirstSynType(mode_.back(), syn->type)) {
        semantic_error_ = true;
        return;
    }
    syn->IncRef();
    factory_.SetFirst(syn);
}
void Generator::SetSecond(std::string_view name) noexcept {
    mode_.pop_back();
    auto itr = synonym_map_.find(name);
    if (itr == synonym_map_.end()) {
        semantic_error_ = true;
        return;
    }
    auto syn = itr->second;
    if (mode_.back() == Mode::kPattern ||
        UnsuitableSecondSynType(mode_.back(), syn->type)) {
        semantic_error_ = true;
        return;
    }
    syn->IncRef();
    factory_.SetSecond(syn);
}
void Generator::Underscore() noexcept {
    if (mode_.back() == Mode::kFirst) {
        mode_.pop_back();
        if (mode_.back() == Mode::kUses || mode_.back() == Mode::kModifies)
            semantic_error_ = true;
        return;
    }
    assert(mode_.back() == Mode::kSecond);
    mode_.pop_back();
    if (mode_.back() == Mode::kPattern) {
        mode_.emplace_back(Mode::kSecond);
        factory_.SetTransPartial();
    }
}
void Generator::Quote() noexcept {
    switch (mode_.back()) {
        case Mode::kExpression:
            mode_.pop_back();
            return factory_.SetSecond(std::move(expression_));
        case Mode::kIdentifier:
            return mode_.resize(mode_.size() - 2);
        case Mode::kZeroth:
            break;
        case Mode::kFirst:
            mode_.emplace_back(Mode::kIdentifier);
            return;
        case Mode::kSecond:
            mode_.emplace_back(*(mode_.rbegin() + 1) == Mode::kPattern
                                       ? Mode::kExpression
                                       : Mode::kIdentifier);
            return expression_.clear();
        default:
            assert(false);
    }
}
void Generator::Comma() noexcept {
    if (mode_.back() == Mode::kDeclaration) {
        return;
    }
    assert(mode_.back() >= Mode::kParent && mode_.back() <= Mode::kPattern);
    mode_.emplace_back(Mode::kSecond);
}
void Generator::Semicolon() noexcept { mode_.pop_back(); }
void Generator::ParseToken(const QueryToken &token) noexcept {
    const auto &[token_type, name] = token;
    switch (token_type) {
        case QueryTokenType::kDeclStmt:
        case QueryTokenType::kDeclRead:
        case QueryTokenType::kDeclPrint:
        case QueryTokenType::kDeclCall:
        case QueryTokenType::kDeclWhile:
        case QueryTokenType::kDeclIf:
        case QueryTokenType::kDeclAssign:
        case QueryTokenType::kDeclVariable:
        case QueryTokenType::kDeclConstant:
        case QueryTokenType::kDeclProcedure:
            return BeginDecl(token_type);
        case QueryTokenType::kKeywordSelect:
            mode_.emplace_back(Mode::kSelect);
            return;
        case QueryTokenType::kKeywordPattern:
        case QueryTokenType::kKeywordFollows:
        case QueryTokenType::kKeywordParent:
        case QueryTokenType::kKeywordUses:
        case QueryTokenType::kKeywordModifies:
        case QueryTokenType::kKeywordCalls:
            return BeginClause(token_type);
        case QueryTokenType::kOperatorPlus:
        case QueryTokenType::kOperatorMinus:
        case QueryTokenType::kOperatorDivide:
        case QueryTokenType::kOperatorModulo:
            expression_.emplace_back(token);
            return;
        case QueryTokenType::kOperatorTimes:
            return Asterisk();
        case QueryTokenType::kBracketL:
            return BracketL();
        case QueryTokenType::kBracketR:
            return BracketR();
        case QueryTokenType::kWord:
            return Name(token);
        case QueryTokenType::kInteger:
            return Constant(token);
        case QueryTokenType::kUnderscore:
            return Underscore();
        case QueryTokenType::kQuote:
            return Quote();
        case QueryTokenType::kComma:
            return Comma();
        case QueryTokenType::kSemicolon:
            return Semicolon();
        case QueryTokenType::kKeywordSuch:
        case QueryTokenType::kKeywordThat:
            break;
        default:
            assert(false);
    }
}
}  // namespace spa
