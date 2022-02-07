#ifndef SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_DECLARATION_H_
#define sSRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_DECLARATION_H_
#include<string>

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

    class Declaration{
        DeclarationType type;
        std::string synonym;
      public:
        Declaration(DeclarationType type, std::string synonym);
        DeclarationType getDeclarationType();
        std::string getDeclarationSynonym();
    };
}
#endif //SPA_TEAM31_CODE31_SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_DECLARATION_H_
