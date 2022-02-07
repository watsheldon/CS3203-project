#include "call.h"

namespace spa {

bool Call::setProcedure(std::shared_ptr<Procedure> procedure) {
	procedure_ = procedure;
	return true;
}

std::shared_ptr<Procedure> Call::getProcedure() {
	return procedure_;
}

} // namespace spa
