#ifndef SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
#define SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
#include "declaration.h"
#include "select.h"
#include <vector>

namespace spa {

    class QueryObject{
      private:
        Declaration declaration;
        Select select;
      public:
        QueryObject(Declaration decl, Select sel);
        Declaration getDeclaration();
        Select getSelect();
    };
}

#endif //SPA_TEAM31_CODE31_SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
