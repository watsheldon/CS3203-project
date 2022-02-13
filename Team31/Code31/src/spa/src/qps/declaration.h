#ifndef SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_DECLARATION_H_
#define SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_DECLARATION_H_
#include<string>
#include <utility>

namespace spa {
enum class DeclarationType{
    STMT,
    READ,
    PRINT,
    CALL,
    WHILE,
    IF,
    ASSIGN,
    VARIABLE,
    CONSTANT,
    PROCEDURE,
};

std::string_view keyword(DeclarationType type) {
    switch(type) {
        case DeclarationType::STMT:
            return "stmt";
        case DeclarationType::READ:
            return "read";
        case DeclarationType::PRINT:
            return "print";
        case DeclarationType::CALL:
            return "call";
        case DeclarationType::WHILE:
            return "while";
        case DeclarationType::IF:
            return "if";
        case DeclarationType::ASSIGN:
            return "assign";
        case DeclarationType::VARIABLE:
            return "variable";
        case DeclarationType::CONSTANT:
            return "constant";
        case DeclarationType::PROCEDURE:
            return "procedure";
    }
}

class Declaration{
    DeclarationType type;
    std::string synonym;
  public:
    Declaration(DeclarationType type, std::string synonym)
            :type(type), synonym(std::move(synonym)) {}
    DeclarationType getDeclarationType() {
        return this->type;
    }
    std::string getDeclarationSynonym() {
        return this->synonym;
    }
};
}
#endif //SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_DECLARATION_H_
