#ifndef SPA_TEAM31_CODE31_SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
#define SPA_TEAM31_CODE31_SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
#include "declaration.h"
#include "select.h"
#include <vector>

namespace spa {

    class QueryObject{
      private:
        bool hasSuchThat;
        bool hasPattern;
        std::vector<Declaration> declarations;
        std::vector<Select> selects;
      public:
        std::vector<Declaration> getDeclarations();
        std::vector<Select> getSelects();
    };
}

#endif //SPA_TEAM31_CODE31_SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
