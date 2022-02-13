#ifndef SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
#define SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_

#include "declaration.h"
#include "pattern.h"
#include "select.h"
#include "such_that.h"
#include <vector>

namespace spa {

    class QueryObject{
      private:
        bool isValid;
        bool hasSuchThat;
        bool hasPattern;
        std::vector<Declaration> declarations;
        Select select;
        SuchThat such_that_;
        Pattern pattern;
      public:
        QueryObject(bool isValid,
                    bool hasSuchThat,
                    bool hasPattern,
                    std::vector<Declaration> declarations,
                    Select select,
                    SuchThat such_that_,
                    Pattern pattern);
        std::vector<Declaration> getDeclarations();
        Select getSelect();
        SuchThat getSuchThat();
        Pattern getPattern();
    };
}

#endif //SPA_TEAM31_CODE31_SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
