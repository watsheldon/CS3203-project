#include "query_object.h"

namespace spa{

QueryObject::QueryObject(bool isValid,
                         bool hasSuchThat,
                         bool hasPattern,
                         std::vector<Declaration> declarations,
                         Select select,
                         SuchThat such_that_,
                         Pattern pattern)
        : isValid(isValid),
          hasSuchThat(hasSuchThat),
          hasPattern(hasPattern),
          declarations(declarations),
          select(select),
          such_that_(such_that_),
          pattern(pattern) {}

std::vector<Declaration> QueryObject::getDeclarations() {
    return this->declarations;
}
Select QueryObject::getSelect() {
    return this->select;
}

SuchThat QueryObject::getSuchThat() {
    return this->such_that_;
}

Pattern QueryObject::getPattern() {
    return this->pattern;
}
}