#include "program.h"

namespace spa {

bool Program::addProcedure(std::shared_ptr<Procedure> procedure) {
	procedures.push_back(procedure);
	return true;
}

SharedPtrVec<Procedure> Program::getProcedures() {
	return procedures;
}

} // namespace spa
