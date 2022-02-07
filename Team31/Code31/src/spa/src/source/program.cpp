#include "program.h"

namespace spa {

bool Program::addProcedure(std::shared_ptr<Procedure> procedure) {
	if (procedure != nullptr) {
		procedures.push_back(procedure);
		return true;
	} else {
		return false;
	}
}

SharedPtrVec<Procedure> Program::getProcedures() {
	return procedures;
}

} // namespace spa