#include "variable.h"

namespace spa {

bool Variable::setName(std::string name) {
	name_ = name;
	return true;
}

std::string Variable::getName() {
	return name_;
}

} // namespace spa
