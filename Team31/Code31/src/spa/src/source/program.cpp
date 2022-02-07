#include "program.h"

namespace spa {

bool addProcedure(std::shared_ptr<Procedure> procedure) {
	if (procedure != nullptr) {
		procedures.push_back(procedure);
		return true;
	} else {
		return false;
	}
}

SharedPtrVec<Procedure> getProcedures() const {
	return procedures;
}

} // namespace spa