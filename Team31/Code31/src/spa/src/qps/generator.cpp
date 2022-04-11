#include "generator.h"

#include <cassert>
#include <memory>

#include "common/algorithm.h"
#include "common/entity_type_enum.h"
#include "conditions/condition_clause.h"

namespace spa {
QueryObject Generator::Generate(const VecTokens &tokens) noexcept {
    Reset();
    for (const auto &token : tokens) {
        ParseToken(token);
        if (semantic_error_ && can_terminate_)
            return QueryObject(std::move(selected_));
    }
    assert(mode_.empty());
    return {std::move(selected_), std::move(synonyms_), std::move(conditions_)};
}
constexpr Synonym::Type Generator::TokenToSynType(
        QueryTokenType type) noexcept {
    assert(type < QueryTokenType::kKeywordSelect);
    return static_cast<Synonym::Type>(type);
}
constexpr Generator::Mode Generator::TokenToClauseMode(
        QueryTokenType type) noexcept {
    assert(type > QueryTokenType::kKeywordAnd &&
           type < QueryTokenType::kAttrProc);
    return Mode{static_cast<int>(type) -
                static_cast<int>(QueryTokenType::kKeywordWith) +
                static_cast<int>(Mode::kWith)};
}
constexpr Attribute Generator::TokenToAttrType(QueryTokenType type) noexcept {
    assert(type >= QueryTokenType::kAttrProc &&
           type <= QueryTokenType::kAttrStmtNum);
    return Attribute{static_cast<int>(type) -
                     static_cast<int>(QueryTokenType::kAttrProc)};
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
        case Mode::kNext:
            return type > Synonym::kStmtAssign;
        case Mode::kAffects:
            return type != Synonym::kStmtAssign && type != Synonym::kStmtAny;
        case Mode::kWith:
            return false;
        default:
            assert(false);
            return false;
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
        case Mode::kNext:
            return type > Synonym::kStmtAssign;
        case Mode::kAffects:
            return type != Synonym::kStmtAssign && type != Synonym::kStmtAny;
        case Mode::kWith:
            return false;
        default:
            assert(false);
            return false;
    }
}
constexpr bool Generator::UnsuitableAttributeType(Synonym *syn,
                                                  Attribute attr) noexcept {
    auto type = syn->type;
    switch (type) {
        case Synonym::kProc:
            return attr != Attribute::kProcName;
        case Synonym::kVar:
            return attr != Attribute::kVarName;
        case Synonym::kConst:
            return attr != Attribute::kValue;
        case Synonym::kStmtCall:
            return attr != Attribute::kProcName && attr != Attribute::kStmtNum;
        case Synonym::kStmtRead:
        case Synonym::kStmtPrint:
            return attr != Attribute::kVarName && attr != Attribute::kStmtNum;
        case Synonym::kStmtAny:
        case Synonym::kStmtWhile:
        case Synonym::kStmtIf:
        case Synonym::kStmtAssign:
            return attr != Attribute::kStmtNum;
        default:
            assert(false);
            return false;
    }
}
void Generator::SetIsWithNum(Attribute attr) noexcept {
    switch (attr) {
        case Attribute::kProcName:
        case Attribute::kVarName:
            is_with_num_ = false;
            return;
        case Attribute::kValue:
        case Attribute::kStmtNum:
            is_with_num_ = true;
            return;
        default:
            assert(false);
    }
}
void Generator::SemanticError() noexcept { semantic_error_ = true; }
void Generator::Reset() noexcept {
    semantic_error_ = false;
    can_terminate_ = false;
    synonym_map_.clear();
    synonyms_.clear();
    selected_.clear();
    mode_.clear();
    curr_syn_type_ = Synonym::kNone;
    first_attr_ = Attribute::kNone;
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
    if (mode_.back() == Mode::kWith) {
        is_left_zero_ = false;
        mode_.emplace_back(Mode::kFirst);
    } else {
        mode_.emplace_back(Mode::kZeroth);
    }
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
        return;
    } else if (mode_.back() == Mode::kSecond) {
        mode_.pop_back();
        bool is_pattern_if = mode_.back() == Mode::kPattern &&
                             pattern_syn_->type == Synonym::kStmtIf;
        return is_pattern_if ? SemanticError() : BracketR();
    }
    assert(mode_.back() > Mode::kSelect && mode_.back() < Mode::kExpression);
    mode_.pop_back();
    auto clause = factory_.Build();
    if (clause == nullptr) return SemanticError();
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
            return Identifier(const_cast<std::string &>(token.value));
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
void Generator::Identifier(const std::string &name) noexcept {
    mode_.pop_back();
    if (mode_.back() == Mode::kFirst) {
        is_with_num_ = false;
        factory_.SetFirst(name);
    } else {
        is_with_num_ ? SemanticError() : factory_.SetSecond(name);
    }
    mode_.emplace_back(Mode::kIdentifier);
}
void Generator::Constant(const QueryToken &token) noexcept {
    const auto &val = token.value;
    if (mode_.back() == Mode::kExpression) {
        expression_.emplace_back(token);
        return;
    }
    // with
    if (*++mode_.rbegin() == Mode::kWith && mode_.back() == Mode::kFirst) {
        is_left_zero_ = std::stoi(val) == 0;
        is_with_num_ = true;
        factory_.SetFirst(val);
        mode_.pop_back();
        return;
    }
    if (*++mode_.rbegin() == Mode::kWith && mode_.back() == Mode::kSecond) {
        if (std::stoi(val) == 0 && first_attr_ == Attribute::kStmtNum)
            return SemanticError();
        mode_.resize(mode_.size() - 2);
        if (!is_with_num_) return SemanticError();
        factory_.SetSecond(val);
        auto clause = factory_.Build();
        conditions_.emplace_back(std::move(clause));
        return;
    }
    // stmt#
    auto value = std::stoi(val);
    if (value == 0) return SemanticError();
    mode_.back() == Mode::kFirst ? factory_.SetFirst(value)
                                 : factory_.SetSecond(value);
    mode_.pop_back();
}
void Generator::AddDecl(std::string_view name) noexcept {
    auto &ptr =
            synonyms_.emplace_back(std::make_unique<Synonym>(curr_syn_type_));
    auto [itr, inserted] = synonym_map_.try_emplace(name, ptr.get());
    if (!inserted) return SemanticError();
}
void Generator::Select(std::string_view name) noexcept {
    can_terminate_ = true;
    auto itr = synonym_map_.find(name);
    Synonym *selected = nullptr;
    if (itr == synonym_map_.end()) {
        if (!selected_.empty() || name != kBoolean || mode_.size() > 1)
            return SemanticError();
    } else {
        selected = itr->second;
    }
    selected_.emplace_back(selected);
    mode_.pop_back();
}
void Generator::Attr(QueryTokenType token_type) noexcept {
    Attribute attribute = TokenToAttrType(token_type);
    if (mode_.back() == Mode::kFirst) {
        mode_.pop_back();
        if (UnsuitableAttributeType(first_with_syn_, attribute))
            return SemanticError();
        SetIsWithNum(attribute);
        first_attr_ = attribute;
        return factory_.SetFirst(attribute);
    }
    if (mode_.back() == Mode::kSecond) {
        mode_.pop_back();
        if (UnsuitableAttributeType(second_with_syn_, attribute))
            return SemanticError();
        if (is_with_num_ && attribute <= Attribute::kVarName)
            return SemanticError();
        if (is_left_zero_ && attribute == Attribute::kStmtNum)
            return SemanticError();
        if (!is_with_num_ && attribute > Attribute::kVarName)
            return SemanticError();
        factory_.SetSecond(attribute);
        mode_.pop_back();
        auto clause = factory_.Build();
        if (clause == nullptr) return SemanticError();
        conditions_.emplace_back(std::move(clause));
        return;
    }
    assert(!selected_.empty() &&
           selected_.back().attribute == Attribute::kNone);
    auto syn = selected_.back().synonym;
    if (UnsuitableAttributeType(syn, TokenToAttrType(token_type)))
        return SemanticError();
    selected_.pop_back();
    selected_.emplace_back(syn, TokenToAttrType(token_type));
}
void Generator::SetZeroth(std::string_view name) noexcept {
    auto itr = synonym_map_.find(name);
    if (itr == synonym_map_.end() || InvalidPatternSyn(itr->second->type)) {
        return SemanticError();
    }
    pattern_syn_ = itr->second;
    pattern_syn_->IncRef();
    factory_.SetPatternSynonym(pattern_syn_);
}
void Generator::SetFirst(std::string_view name) noexcept {
    mode_.pop_back();
    auto itr = synonym_map_.find(name);
    if (itr == synonym_map_.end()) return SemanticError();
    auto syn = itr->second;
    if (UnsuitableFirstSynType(mode_.back(), syn->type)) return SemanticError();
    syn->IncRef();
    factory_.SetFirst(syn);
    if (mode_.back() == Mode::kWith) {
        first_with_syn_ = syn;
        mode_.emplace_back(Mode::kFirst);
    }
}
void Generator::SetSecond(std::string_view name) noexcept {
    mode_.pop_back();
    if (mode_.back() == Mode::kPattern) return SemanticError();
    auto itr = synonym_map_.find(name);
    if (itr == synonym_map_.end()) return SemanticError();
    auto syn = itr->second;
    if (UnsuitableSecondSynType(mode_.back(), syn->type))
        return SemanticError();
    syn->IncRef();
    factory_.SetSecond(syn);
    if (mode_.back() == Mode::kWith) {
        second_with_syn_ = syn;
        mode_.emplace_back(Mode::kSecond);
    }
}
void Generator::Underscore() noexcept {
    if (mode_.back() == Mode::kFirst) {
        mode_.pop_back();
        if (mode_.back() == Mode::kUses || mode_.back() == Mode::kModifies)
            SemanticError();
        return;
    }
    if (mode_.back() == Mode::kThird) {
        mode_.pop_back();
        return;
    }
    assert(mode_.back() == Mode::kSecond);
    mode_.pop_back();
    if (mode_.back() == Mode::kPattern) {
        mode_.emplace_back(Mode::kSecond);
        factory_.SetTransPartial();
    }
}
void Generator::QuoteAsSecondArg() noexcept {
    if (*++mode_.rbegin() != Mode::kPattern) {
        mode_.emplace_back(Mode::kIdentifier);
        return;
    }
    if (pattern_syn_->type == Synonym::kStmtWhile ||
        pattern_syn_->type == Synonym::kStmtIf)
        return SemanticError();
    mode_.emplace_back(Mode::kExpression);
    expression_.clear();
}
void Generator::Quote() noexcept {
    switch (mode_.back()) {
        case Mode::kExpression:
            mode_.pop_back();
            return factory_.SetSecond(std::move(expression_));
        case Mode::kIdentifier:
            return RightQuote();
        case Mode::kZeroth:
            break;
        case Mode::kFirst:
            mode_.emplace_back(Mode::kIdentifier);
            return;
        case Mode::kSecond:
            return QuoteAsSecondArg();
        default:
            assert(false);
    }
}
void Generator::RightQuote() noexcept {
    mode_.pop_back();
    bool is_second = mode_.back() == Mode::kSecond;
    mode_.pop_back();
    bool is_with = mode_.back() == Mode::kWith;
    if (is_second && is_with) {
        auto clause = factory_.Build();
        if (clause == nullptr) return SemanticError();
        mode_.pop_back();
        conditions_.emplace_back(std::move(clause));
    }
}
void Generator::Comma() noexcept {
    if (mode_.back() == Mode::kDeclaration) return;
    if (mode_.back() == Mode::kMultiSelect) {
        mode_.emplace_back(Mode::kSelect);
        return;
    }
    if (mode_.back() == Mode::kSecond &&
        pattern_syn_->type == Synonym::kStmtIf) {
        mode_.back() = Mode::kThird;
        return;
    }
    if (mode_.back() == Mode::kSecond) {
        return SemanticError();
    }
    if (mode_.back() > Mode::kSelect && mode_.back() < Mode::kExpression) {
        mode_.emplace_back(Mode::kSecond);
        return;
    }
    assert(false);
}
void Generator::Semicolon() noexcept { mode_.pop_back(); }
constexpr bool Generator::InvalidPatternSyn(Synonym::Type syn_type) noexcept {
    return spa::find(kPatternSynonymType.begin(), kPatternSynonymType.end(),
                     syn_type) == kPatternSynonymType.end();
}
void Generator::AngleBracketL() noexcept {
    assert(mode_.back() == Mode::kSelect);
    mode_.back() = Mode::kMultiSelect;
    mode_.emplace_back(Mode::kSelect);
}
void Generator::AngleBracketR() noexcept {
    assert(mode_.back() == Mode::kMultiSelect);
    mode_.pop_back();
}
void Generator::SetClauseMode(QueryTokenType token_type) noexcept {
    switch (token_type) {
        case QueryTokenType::kKeywordSuch:
            curr_clause_mode_ = ClauseMode::kSuchThat;
            return;
        case QueryTokenType::kKeywordWith:
            curr_clause_mode_ = ClauseMode::kWith;
            return;
        case QueryTokenType::kKeywordPattern:
            curr_clause_mode_ = ClauseMode::kPattern;
            return;
        default:
            assert(false);
            return;
    }
}
void Generator::And() noexcept {
    switch (curr_clause_mode_) {
        case ClauseMode::kSuchThat:
            return;
        case ClauseMode::kPattern:
            return BeginClause(QueryTokenType::kKeywordPattern);
        case ClauseMode::kWith:
            return BeginClause(QueryTokenType::kKeywordWith);
        default:
            assert(false);
            return;
    }
}
void Generator::Equal() noexcept {
    assert(mode_.back() == Mode::kWith);
    mode_.emplace_back(Mode::kSecond);
    return;
}
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
        case QueryTokenType::kDeclProcedure:
        case QueryTokenType::kDeclVariable:
        case QueryTokenType::kDeclConstant:
            return BeginDecl(token_type);
        case QueryTokenType::kKeywordSelect:
            mode_.emplace_back(Mode::kSelect);
            return;
        case QueryTokenType::kKeywordPattern:
        case QueryTokenType::kKeywordWith:
            SetClauseMode(token_type);
        case QueryTokenType::kKeywordFollows:
        case QueryTokenType::kKeywordParent:
        case QueryTokenType::kKeywordUses:
        case QueryTokenType::kKeywordModifies:
        case QueryTokenType::kKeywordCalls:
        case QueryTokenType::kKeywordNext:
        case QueryTokenType::kKeywordAffects:
            return BeginClause(token_type);
        case QueryTokenType::kAttrProc:
        case QueryTokenType::kAttrVar:
        case QueryTokenType::kAttrValue:
        case QueryTokenType::kAttrStmtNum:
            return Attr(token_type);
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
        case QueryTokenType::kAngleBracketL:
            return AngleBracketL();
        case QueryTokenType::kAngleBracketR:
            return AngleBracketR();
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
        case QueryTokenType::kKeywordAnd:
            return And();
        case QueryTokenType::kKeywordSuch:
            return SetClauseMode(token_type);
        case QueryTokenType::kEqual:
            return Equal();
        case QueryTokenType::kKeywordThat:
        case QueryTokenType::kDot:
            return;
        default:
            assert(false);
    }
}
}  // namespace spa
