#include "variable.h"

namespace spa {

bool Variable::setName(std::string name) {
	name = name;
	return true;
}

std::string Variable::getName() {
	return name;
}

} // namespace spa