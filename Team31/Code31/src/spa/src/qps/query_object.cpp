#include "query_object.h"

namespace spa{
QueryObject::QueryObject(std::vector<Declaration> decls, Select sel) : select(sel),
                                                                       declarations(decls) {}


std::vector<Declaration> QueryObject::getDeclarations(){
    return declarations;
}
Select QueryObject::getSelect(){
    return select;
}
}