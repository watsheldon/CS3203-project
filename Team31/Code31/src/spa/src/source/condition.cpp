#include "condition.h"

namespace spa {

bool Condition::addVariable(std::shared_ptr<Variable> variable) {
	variables.push_back(variable);
	return true;
}

bool Condition::addConstant(std::shared_ptr<Constant> constant) {
	constants.push_back(constant);
	return true;
}

SharedPtrVec<Variable> Condition::getVariables() {
	return variables;
}

SharedPtrVec<Constant> Condition::getConstants() {
	return constants;
}

} // namespace spa
