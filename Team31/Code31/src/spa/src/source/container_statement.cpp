#include "container_statement.h"

namespace spa {

ContainerStatement::~ContainerStatement() {}

bool ContainerStatement::setCondition(std::shared_ptr<Condition> condition) {
	condition_ = condition;
	return true;
}

std::shared_ptr<Condition> ContainerStatement::getCondition() {
	return condition_;
}

} // namespace spa
