#include "declaration.h"

using namespace spa;

DeclarationType Declaration::getDeclarationType() {
    return type;
}
std::string Declaration::getDeclarationSynonym(){
    return synonym;
}