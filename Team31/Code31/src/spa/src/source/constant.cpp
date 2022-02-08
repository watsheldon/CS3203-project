#include "constant.h"

namespace spa {

bool Constant::setValue(std::string value) {
	value_ = value;
	return true;
}

std::string Constant::getValue() {
	return value_;
}

} // namespace spa
