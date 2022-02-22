#include "generator.h"

#include <cassert>

#include "conditions/condition_clause.h"
#include "conditions/factory.h"

namespace spa {

QueryObject Generator::Generate(const std::vector<QueryToken> &tokens) {
    std::map<std::string, std::unique_ptr<Synonym>> map;
    Mode curr_mode_;
    DeclarationType curr_type_;
    std::vector<std::unique_ptr<Synonym>> synonyms_;
    std::vector<std::unique_ptr<ConditionClause>> conditions;
    Factory factory;
    QueryObjectBuilder query_object_builder_;

    for (const auto &[type, value] : tokens) {
        switch (type) {
            case QueryTokenType::STMT:
            case QueryTokenType::READ:
            case QueryTokenType::PRINT:
            case QueryTokenType::CALL:
            case QueryTokenType::WHILE:
            case QueryTokenType::IF:
            case QueryTokenType::ASSIGN:
            case QueryTokenType::VARIABLE:
            case QueryTokenType::CONSTANT:
            case QueryTokenType::PROCEDURE:
                curr_mode_ = Mode::kDeclaration;
                curr_type_ = TypeConvert(type);
                break;
            case QueryTokenType::SELECT:
                curr_mode_ = Mode::kSelect;
                break;
            case QueryTokenType::PATTERN:
                curr_mode_ = Mode::kPattern;
                break;
            case QueryTokenType::WORD:
                if (curr_mode_ == Mode::kDeclaration) {
                    std::unique_ptr<Synonym> ptr =
                            std::make_unique<Synonym>(curr_type_);
                    auto pair = map.try_emplace(value, std::move(ptr));
                    // do not insert means repeated synonym
                    if (!pair.second) {
                        query_object_builder_.SetIsValid(false);
                        return query_object_builder_.build();
                    }
                    synonyms_.emplace_back(std::move(ptr));
                }
                if (curr_mode_ == Mode::kSelect) {
                    auto it = map.find(value);
                    if (it == map.end()) {
                        // this synonym is not declared before
                        query_object_builder_.SetIsValid(false);
                        return query_object_builder_.build();
                    }
                    auto &[name, synonym] = *it;
                    // todo: not sure syntax
                    query_object_builder_.SetSelect(std::move(synonym));
                }
                // todo: check the synonym type in diff relationship
                if (curr_mode_ == Mode::kParent) {
                }
                if (curr_mode_ == Mode::kFollows) {
                }
                if (curr_mode_ == Mode::kUses) {
                }
                if (curr_mode_ == Mode::kModifies) {
                }
                if (curr_mode_ == Mode::kPattern) {
                }
            case QueryTokenType::COMMA:
            case QueryTokenType::SEMICOLON:
            case QueryTokenType::PLUS:
            case QueryTokenType::MINUS:
            case QueryTokenType::DIVIDE:
            case QueryTokenType::MODULO:
            case QueryTokenType::LEFTBRACKET:
            case QueryTokenType::THAT:
                break;
            case QueryTokenType::RIGHTBRACKET:
                // todo:factory build (syntax of unique_ptr)
                conditions.emplace_back(factory.Build());
                break;
            case QueryTokenType::UNDERSCORE:
                if (curr_mode_ == Mode::kParent) {
                }
                if (curr_mode_ == Mode::kFollows) {
                }
                if (curr_mode_ == Mode::kUses) {
                }
                if (curr_mode_ == Mode::kModifies) {
                }
                if (curr_mode_ == Mode::kExpression) {
                }
            case QueryTokenType::TIMES:
                if (curr_mode_ == Mode::kFollows ||
                    curr_mode_ == Mode::kParent) {
                    factory.SetTrans(true);
                }
                break;
            case QueryTokenType::SUCH:
                break;
            case QueryTokenType::FOLLOWS:
                curr_mode_ = Mode::kFollows;
                factory.SetRelationship(type);
                break;
            case QueryTokenType::PARENT:
                curr_mode_ = Mode::kParent;
                factory.SetRelationship(type);
                break;
            case QueryTokenType::USES:
                curr_mode_ = Mode::kUses;
                factory.SetRelationship(type);
                break;
            case QueryTokenType::MODIFIES:
                curr_mode_ = Mode::kModifies;
                factory.SetRelationship(type);
                break;
            case QueryTokenType::DOUBLEQUOTE:
                break;
            case QueryTokenType::INTEGER:
                break;
        }
    }
    return query_object_builder_.build();
}

DeclarationType Generator::TypeConvert(QueryTokenType type) {
    switch (type) {
        case QueryTokenType::STMT:
            return DeclarationType::STMT;
        case QueryTokenType::READ:
            return DeclarationType::READ;
        case QueryTokenType::PRINT:
            return DeclarationType::PRINT;
        case QueryTokenType::CALL:
            return DeclarationType::CALL;
        case QueryTokenType::WHILE:
            return DeclarationType::WHILE;
        case QueryTokenType::IF:
            return DeclarationType::IF;
        case QueryTokenType::ASSIGN:
            return DeclarationType::ASSIGN;
        case QueryTokenType::VARIABLE:
            return DeclarationType::VARIABLE;
        case QueryTokenType::CONSTANT:
            return DeclarationType::CONSTANT;
        case QueryTokenType::PROCEDURE:
            return DeclarationType::PROCEDURE;
        default:
            assert(false);
    }
}
bool Generator::GenerateDeclarations() {
    DeclarationType curr_type_;
    //    if (comma) {
    //        addSynonym()
    //    }
    //    isValid
    return false;
}
bool Generator::GenerateSelect() {
    std::string value;
    //    checkMapContains(value);
    //    isValid;
    //    setSelect()
    return false;
}
bool Generator::GenerateSuchThat() { return false; }
}  // namespace spa

// namespace spa
