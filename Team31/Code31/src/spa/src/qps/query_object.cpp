#include "query_object.h"

namespace spa{
QueryObject::QueryObject() : isValidQuery(true), hasSuchThatClause(false), hasPatternClause(false) {}

std::vector<DeclarationClause> QueryObject::getDeclarations() {
    return this->declarations;
}
SelectClause QueryObject::getSelect() {
    return this->select;
}

SuchThatClause QueryObject::getSuchThat() {
    return this->such_that_;
}

PatternClause QueryObject::getPattern() {
    return this->pattern;
}

bool QueryObject::isValid() {
    return isValidQuery;
}
bool QueryObject::hasSuchThat() {
    return hasSuchThatClause;
}
bool QueryObject::hasPattern() {
    return hasPatternClause;
}
void QueryObject::addDeclaration(DeclarationType type, std::string synonym) {
    DeclarationClause declaration_cl;
    declaration_cl.setDeclarationType(type);
    declaration_cl.setSynonym(synonym);
    declarations.emplace_back(declaration_cl);
}
void QueryObject::setSelect(DeclarationType type, std::string synonym) {
    select.setSelectType(type);
    select.setSynonym(synonym);
}

void QueryObject::setSuchThat(SuchThatType type, std::string ref1, std::string ref2) {
    such_that_.setType(type);
    such_that_.setRef1(ref1);
    such_that_.setRef2(ref2);
}
void QueryObject::setPattern(std::string syn, std::string ref1, std::string ref2) {
    pattern.setSynonym(syn);
    pattern.setRef1(ref1);
    pattern.setRef2(ref2);
}
void QueryObject::setIsValid(bool isValid) {
    isValidQuery = isValid;
}
void QueryObject::setHasSuchThat(bool hasSuchThat) {
    hasSuchThatClause = hasSuchThat;
}
void QueryObject::setHasPattern(bool hasPattern) {
    hasPatternClause = hasPattern;
}
}