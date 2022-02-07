#include "select.h"

using namespace spa;

Select::Select(DeclarationType s_type, std::string s_synonym) {
    type = s_type;
    synonym = s_synonym;
}
DeclarationType Select::getSelectType(){
    return type;
}
std::string Select::getSelectSynonym(){
    return synonym;
}