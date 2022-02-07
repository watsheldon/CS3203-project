#include "call.h"

namespace spa {

bool Call::setProcedure(std::shared_ptr<Procedure> procedure) {
	procedure = procedure;
	return true;
}

std::shared_ptr<Procedure> Call::getPocedure() {
	return procedure;
}

} // namespace spa