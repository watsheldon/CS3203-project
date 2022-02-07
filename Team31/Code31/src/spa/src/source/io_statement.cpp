#include "io_statement.h"

namespace spa {

bool IoStatement::setVariable(std::shared_ptr<Variable> variable) {
	variable = variable;
	return true;
}

int IoStatement::getVariableIndex() const {
	return variable->getIndex();
}

} // namespace spa