#include "query_object.h"

using namespace spa;

std::vector<Declaration> QueryObject::getDeclarations(){
    return declarations;
}
std::vector<Select> QueryObject::getSelects(){
    return selects;
}


