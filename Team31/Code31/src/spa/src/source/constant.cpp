#include "constant.h"

namespace spa {

bool Constant::setName(std::string name) {
	name = name;
	return true;
}

std::string Constant::getName() {
	return name;
}

} // namespace spa