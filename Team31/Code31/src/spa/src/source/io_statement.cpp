#include "io_statement.h"

namespace spa {

IoStatement::~IoStatement() {}

bool IoStatement::setVariable(std::shared_ptr<Variable> variable) {
	variable_ = variable;
	return true;
}

int IoStatement::getVariableIndex() const {
	return variable_->getIndex();
}

} // namespace spa
