#include "generator.h"

#include "query_object_builder.h"

namespace spa {
Generator::Generator(std::filesystem::path &filepath) : validator_(filepath) {}

QueryObject Generator::Generate() {
    tokens = validator_.Validate();
    Mode curr_mode_;
    DeclarationType curr_type_;
    QueryObjectBuilder builder;

    for (const auto &[type, value] : *tokens) {
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
            case QueryTokenType::THAT:
            case QueryTokenType::PATTERN:
                curr_mode_ = Mode::kCondition;
                break;
            case QueryTokenType::WORD:
                if (curr_mode_ == Mode::kDeclaration) {
                    std::unique_ptr<Synonym> ptr1 =
                            std::make_unique<Synonym>(curr_type_);
                    auto pair = map.try_emplace(value, std::move(ptr1));
                    // do not insert means repeated synonym
                    //                    if (!pair.second) {
                    //
                    //                    }
                }
                if (curr_mode_ == Mode::kSelect) {
                }
                if (curr_mode_ == Mode::kCondition) {
                }
            case QueryTokenType::COMMA:
            case QueryTokenType::SEMICOLON:
            case QueryTokenType::PLUS:
            case QueryTokenType::MINUS:
            case QueryTokenType::TIMES:
            case QueryTokenType::DIVIDE:
            case QueryTokenType::MODULO:
            case QueryTokenType::LEFTBRACKET:
            case QueryTokenType::RIGHTBRACKET:
                // Todo: for future iterations, deal with expression mode
                break;
            case QueryTokenType::UNDERSCORE:
                if (curr_mode_ == Mode::kCondition) {
                }
                if (curr_mode_ == Mode::kExpression) {
                }
        }
    }
    return builder.build();
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
