#include "generator.h"

#include <cassert>
#include <map>
#include <memory>
#include <stack>

#include "common/aliases.h"
#include "conditions/condition_clause.h"
#include "conditions/factory.h"

namespace spa {

std::unique_ptr<QueryObject> Generator::Generate(
        const std::vector<QueryToken> &tokens) {
    std::map<std::string_view, Synonym *const> synonym_map;
    VecUniquePtr<Synonym> synonyms;
    Synonym *selected = nullptr;
    std::stack<Mode> mode;
    Synonym::Type curr_syn_type = Synonym::kNone;
    VecUniquePtr<ConditionClause> conditions;
    std::vector<QueryToken> expression;
    Factory factory;

    for (const auto &token : tokens) {
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
                mode.emplace(Mode::kDeclaration);
                curr_syn_type = TokenToSynType(token_type);
                continue;
            case QueryTokenType::kKeywordSelect:
                mode.emplace(Mode::kSelect);
                continue;
            case QueryTokenType::kKeywordPattern:
            case QueryTokenType::kKeywordFollows:
            case QueryTokenType::kKeywordParent:
            case QueryTokenType::kKeywordUses:
            case QueryTokenType::kKeywordModifies:
                factory.SetRelationship(token_type);
                mode.emplace(TokenToClauseMode(token_type));
                mode.emplace(Mode::kZero);
                continue;
            case QueryTokenType::kOperatorPlus:
            case QueryTokenType::kOperatorMinus:
            case QueryTokenType::kOperatorDivide:
            case QueryTokenType::kOperatorModulo:
                assert(mode.top() == Mode::kExpression);
                expression.emplace_back(token);
                continue;
            case QueryTokenType::kOperatorTimes:
                if (mode.top() == Mode::kZero) {
                    factory.SetTransPartial();
                } else if (mode.top() == Mode::kExpression) {
                    expression.emplace_back(token);
                } else {
                    assert(false);
                }
                continue;
            case QueryTokenType::kBracketL:
                if (mode.top() == Mode::kZero) {
                    mode.pop();
                    mode.emplace(Mode::kFirst);
                } else if (mode.top() == Mode::kExpression) {
                    expression.emplace_back(token);
                } else {
                    assert(false);
                }
                continue;
            case QueryTokenType::kBracketR:
                if (mode.top() == Mode::kExpression) {
                    expression.emplace_back(token);
                } else if (mode.top() >= Mode::kParent &&
                           mode.top() <= Mode::kPattern) {
                    mode.pop();
                    std::unique_ptr<ConditionClause> clause = factory.Build();
                    if (clause == nullptr) return {};
                    conditions.emplace_back(std::move(clause));
                } else if (mode.top() == Mode::kSecond) {
                    mode.pop();
                    assert(mode.top() == Mode::kPattern);
                    mode.pop();
                    std::unique_ptr<ConditionClause> clause = factory.Build();
                    if (clause == nullptr) return {};
                    conditions.emplace_back(std::move(clause));
                } else {
                    assert(false);
                }
                continue;
            case QueryTokenType::kWord:
                if (mode.top() == Mode::kDeclaration) {
                    synonyms.emplace_back(
                            std::make_unique<Synonym>(curr_syn_type));
                    auto [itr, inserted] = synonym_map.try_emplace(
                            name, synonyms.back().get());
                    if (!inserted) return {};
                } else if (mode.top() == Mode::kSelect) {
                    auto itr = synonym_map.find(name);
                    if (itr == synonym_map.end()) return {};
                    selected = itr->second;
                    mode.pop();
                } else if (mode.top() == Mode::kZero) {
                    auto itr = synonym_map.find(name);
                    if (itr == synonym_map.end() ||
                        itr->second->type != Synonym::kStmtAssign) {
                        return {};
                    }
                    auto syn = itr->second;
                    syn->IncRef();
                    factory.SetAssign(syn);
                } else if (mode.top() == Mode::kFirst) {
                    mode.pop();
                    auto itr = synonym_map.find(name);
                    if (itr == synonym_map.end()) return {};
                    auto syn = itr->second;
                    if (UnsuitableFirstSynType(mode.top(), syn->type))
                        return {};
                    syn->IncRef();
                    factory.SetFirst(syn);
                } else if (mode.top() == Mode::kSecond) {
                    mode.pop();
                    auto itr = synonym_map.find(name);
                    if (itr == synonym_map.end()) return {};
                    auto syn = itr->second;
                    if (mode.top() == Mode::kPattern ||
                        UnsuitableSecondSynType(mode.top(), syn->type))
                        return {};
                    syn->IncRef();
                    factory.SetSecond(syn);
                } else if (mode.top() == Mode::kExpression) {
                    expression.emplace_back(token);
                } else if (mode.top() == Mode::kIdentifier) {
                    mode.pop();
                    mode.top() == Mode::kFirst ? factory.SetFirst(name)
                                               : factory.SetSecond(name);
                    mode.emplace(Mode::kIdentifier);
                } else {
                    assert(false);
                }
                continue;
            case QueryTokenType::kInteger:
                if (mode.top() == Mode::kFirst) {
                    mode.pop();
                    auto value = std::stoi(name);
                    if (value == 0) return {};
                    factory.SetFirst(value);
                } else if (mode.top() == Mode::kSecond) {
                    mode.pop();
                    auto value = std::stoi(name);
                    if (value == 0) return {};
                    factory.SetSecond(value);
                } else if (mode.top() == Mode::kExpression) {
                    expression.emplace_back(token);
                } else {
                    assert(false);
                }
                continue;
            case QueryTokenType::kUnderscore:
                if (mode.top() == Mode::kFirst) {
                    mode.pop();
                    if (mode.top() == Mode::kUses ||
                        mode.top() == Mode::kModifies) {
                        return {};
                    }
                } else if (mode.top() == Mode::kSecond) {
                    mode.pop();
                    if (mode.top() == Mode::kPattern) {
                        mode.emplace(Mode::kSecond);
                        factory.SetTransPartial();
                        continue;
                    }
                } else {
                    assert(false);
                }
                continue;
            case QueryTokenType::kQuote:
                if (mode.top() == Mode::kFirst) {
                    mode.emplace(Mode::kIdentifier);
                } else if (mode.top() == Mode::kSecond) {
                    mode.pop();
                    auto tmp = mode.top() == Mode::kPattern ? Mode::kExpression
                                                            : Mode::kIdentifier;
                    mode.emplace(Mode::kSecond);
                    mode.emplace(tmp);
                    expression.clear();
                } else if (mode.top() == Mode::kExpression) {
                    mode.pop();
                    factory.SetSecond(std::move(expression));
                } else if (mode.top() == Mode::kIdentifier) {
                    mode.pop();
                    mode.pop();
                } else {
                    assert(false);
                }
                continue;
            case QueryTokenType::kComma:
                if (mode.top() == Mode::kDeclaration) {
                    continue;
                } else if (mode.top() >= Mode::kParent &&
                           mode.top() <= Mode::kPattern) {
                    mode.emplace(Mode::kSecond);
                } else {
                    assert(false);
                }
                continue;
            case QueryTokenType::kSemicolon:
                mode.pop();
                curr_syn_type = Synonym::kNone;
                continue;
            case QueryTokenType::kKeywordSuch:
            case QueryTokenType::kKeywordThat:
                continue;
        }
    }
    return std::make_unique<QueryObject>(selected, std::move(synonyms),
                                         std::move(conditions));
}

constexpr Synonym::Type Generator::TokenToSynType(QueryTokenType type) {
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
constexpr Generator::Mode Generator::TokenToClauseMode(QueryTokenType type) {
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
        default:
            assert(false);
    }
}
constexpr bool Generator::UnsuitableFirstSynType(Generator::Mode mode,
                                                 Synonym::Type type) {
    switch (mode) {
        case Mode::kParent:
            return type != Synonym::kStmtIf && type != Synonym::kStmtWhile &&
                   type != Synonym::kStmtAny;
        case Mode::kFollows:
            return type > Synonym::kStmtAssign;
        case Mode::kUses:
            return type > Synonym::kStmtAssign || type == Synonym::kStmtRead;
        case Mode::kModifies:
            return type > Synonym::kStmtAssign || type == Synonym::kStmtPrint;
        case Mode::kPattern:
            return type != Synonym::kVar;
        default:
            assert(false);
    }
}
constexpr bool Generator::UnsuitableSecondSynType(Generator::Mode mode,
                                                  Synonym::Type type) {
    switch (mode) {
        case Mode::kParent:
        case Mode::kFollows:
            return type > Synonym::kStmtAssign;
        case Mode::kUses:
        case Mode::kModifies:
            return type != Synonym::kVar;
        default:
            assert(false);
    }
}
}  // namespace spa
