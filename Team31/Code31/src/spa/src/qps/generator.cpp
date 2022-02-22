#include "generator.h"

#include <cassert>
#include <map>
#include <memory>

#include "conditions/condition_clause.h"
#include "conditions/factory.h"

namespace spa {

std::unique_ptr<QueryObject> Generator::Generate(
        const std::vector<QueryToken> &tokens) {
    std::map<std::string, std::unique_ptr<Synonym>> map;
    Mode curr_mode_;
    DeclarationType curr_type_;
    std::vector<std::unique_ptr<Synonym>> synonyms_;
    std::vector<std::unique_ptr<ConditionClause>> conditions;
    Factory factory;
    QueryObjectBuilder query_object_builder_;

    for (const auto &[type, value] : tokens) {
        switch (type) {
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
                curr_mode_ = Mode::kDeclaration;
                curr_type_ = TypeConvert(type);
                break;
            case QueryTokenType::kKeywordSelect:
                curr_mode_ = Mode::kSelect;
                break;
            case QueryTokenType::kKeywordPattern:
                curr_mode_ = Mode::kPattern;
                break;
            case QueryTokenType::kWord:
                if (curr_mode_ == Mode::kDeclaration) {
                    std::unique_ptr<Synonym> ptr =
                            std::make_unique<Synonym>(curr_type_);
                    auto pair = map.try_emplace(value, std::move(ptr));
                    // do not insert means repeated synonym
                    if (!pair.second) {
                        query_object_builder_.SetIsValid(false);
                        return {};
                    }
                    synonyms_.emplace_back(std::move(ptr));
                }
                if (curr_mode_ == Mode::kSelect) {
                    auto it = map.find(value);
                    if (it == map.end()) {
                        // this synonym is not declared before
                        query_object_builder_.SetIsValid(false);
                        return {};
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
            case QueryTokenType::kComma:
            case QueryTokenType::kSemicolon:
            case QueryTokenType::kOperatorPlus:
            case QueryTokenType::kOperatorMinus:
            case QueryTokenType::kOperatorDivide:
            case QueryTokenType::kOperatorModulo:
            case QueryTokenType::kBracketL:
            case QueryTokenType::kKeywordThat:
                break;
            case QueryTokenType::kBracketR: {
                // todo:factory build (syntax of unique_ptr)
                auto cond = factory.Build();
                conditions.emplace_back(std::move(cond));
                break;
            }
            case QueryTokenType::kUnderscore:
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
            case QueryTokenType::kOperatorTimes:
                if (curr_mode_ == Mode::kFollows ||
                    curr_mode_ == Mode::kParent) {
                    factory.SetTrans(true);
                }
                break;
            case QueryTokenType::kKeywordSuch:
                break;
            case QueryTokenType::kKeywordFollows:
                curr_mode_ = Mode::kFollows;
                factory.SetRelationship(type);
                break;
            case QueryTokenType::kKeywordParent:
                curr_mode_ = Mode::kParent;
                factory.SetRelationship(type);
                break;
            case QueryTokenType::kKeywordUses:
                curr_mode_ = Mode::kUses;
                factory.SetRelationship(type);
                break;
            case QueryTokenType::kKeywordModifies:
                curr_mode_ = Mode::kModifies;
                factory.SetRelationship(type);
                break;
            case QueryTokenType::kQuote:
                break;
            case QueryTokenType::kInteger:
                break;
        }
    }
    return query_object_builder_.Build();
}

DeclarationType Generator::TypeConvert(QueryTokenType type) {
    switch (type) {
        case QueryTokenType::kDeclStmt:
            return DeclarationType::STMT;
        case QueryTokenType::kDeclRead:
            return DeclarationType::READ;
        case QueryTokenType::kDeclPrint:
            return DeclarationType::PRINT;
        case QueryTokenType::kDeclCall:
            return DeclarationType::CALL;
        case QueryTokenType::kDeclWhile:
            return DeclarationType::WHILE;
        case QueryTokenType::kDeclIf:
            return DeclarationType::IF;
        case QueryTokenType::kDeclAssign:
            return DeclarationType::ASSIGN;
        case QueryTokenType::kDeclVariable:
            return DeclarationType::VARIABLE;
        case QueryTokenType::kDeclConstant:
            return DeclarationType::CONSTANT;
        case QueryTokenType::kDeclProcedure:
            return DeclarationType::PROCEDURE;
        default:
            assert(false);
    }
}
// bool Generator::GenerateDeclarations() {
//     DeclarationType curr_type_;
//     //    if (comma) {
//     //        addSynonym()
//     //    }
//     //    isValid
//     return false;
// }
// bool Generator::GenerateSelect() {
//     std::string value;
//     //    checkMapContains(value);
//     //    isValid;
//     //    setSelect()
//     return false;
// }
// bool Generator::GenerateSuchThat() { return false; }
}  // namespace spa

// namespace spa
