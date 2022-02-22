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

struct Synonym {
    const DeclarationType type;
    int count = 0;

  public:
    explicit Synonym(DeclarationType t) : type(t) {}
    void RefCountSetter(int num) { count++; }
};
}  // namespace spa
#endif  // SRC_SPA_SRC_QPS_SYNONYM_H_
