#include "procedure.h"

namespace spa {

bool Procedure::setStmtLst(std::shared_ptr<StmtLst> stmtLst) {
	if (stmtLst != nullptr) {
		stmtLst = stmtLst;
		return true;
	} else {
		return false;
	}
}

std::shared_ptr<StmtLst> getStmtLst() const {
	return stmtLst;
}

} // namespace spa