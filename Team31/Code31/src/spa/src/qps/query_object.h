#ifndef SPA_TEAM31_CODE31_SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
#define SPA_TEAM31_CODE31_SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
#include "declaration.h"
#include "select.h"
#include <vector>

namespace spa {

    class QueryObject{
      private:
        std::vector<Declaration> declarations;
        Select select;
      public:
        QueryObject(std::vector<Declaration> decls, Select sel, Select select);
        std::vector<Declaration> getDeclarations();
        Select getSelect();
        QueryObject(std::vector<Declaration> vector_1, Select select);
    };
}

#endif //SPA_TEAM31_CODE31_SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
