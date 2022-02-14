#ifndef SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
#define SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_

#include "declaration_cl.h"
#include "pattern_cl.h"
#include "select_cl.h"
#include "such_that_cl.h"
#include <vector>

namespace spa {
class QueryObject{
  private:
    bool isValidQuery;
    bool hasSuchThatClause;
    bool hasPatternClause;
    std::vector<DeclarationClause> declarations;
    SelectClause select;
    SuchThatClause such_that_;
    PatternClause pattern;
  public:
    QueryObject();

    // getters
    std::vector<DeclarationClause> getDeclarations();
    SelectClause getSelect();
    SuchThatClause getSuchThat();
    PatternClause getPattern();
    bool isValid();
    bool hasSuchThat();
    bool hasPattern();

    // setters
    void addDeclaration(DeclarationType type, std::string synonym);
    void setSelect(DeclarationType type, std::string synonym);
    void setSuchThat(SuchThatType type, std::string ref1, std::string ref2);
    void setPattern(std::string syn, std::string ref1, std::string ref2);
    void setIsValid(bool isValid);
    void setHasSuchThat(bool hasSuchThat);
    void setHasPattern(bool hasPattern);
};
}

#endif //SPA_TEAM31_CODE31_SRC_SPA_SRC_QUERYPROCESSINGSUBSYSTEM_QUERYOBJECT_H_
