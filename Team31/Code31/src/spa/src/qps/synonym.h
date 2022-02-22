#ifndef SRC_SPA_SRC_QPS_SYNONYM_H_
#define SRC_SPA_SRC_QPS_SYNONYM_H_

#include <string_view>
namespace spa {

enum class DeclarationType {
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
    switch (type) {
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

struct Synonym {
    const DeclarationType type;
    int count = 0;

  public:
    explicit Synonym(DeclarationType t) : type(t) {}
    void RefCountSetter(int num) { count++; }
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_SYNONYM_H_
