#include "declaration.h"

using namespace spa;

Declaration::Declaration(DeclarationType d_type, std::string d_synonym) {
    type = d_type;
    synonym = d_synonym;
}

DeclarationType Declaration::getDeclarationType() {
    return type;
}

std::string Declaration::getDeclarationSynonym(){
    return synonym;
}