#include "query_object.h"

namespace spa{
QueryObject::QueryObject(Declaration decl, Select sel) : select(sel), declaration(decl) {}


Declaration QueryObject::getDeclaration(){
    return declaration;
}
Select QueryObject::getSelect(){
    return select;
}
}