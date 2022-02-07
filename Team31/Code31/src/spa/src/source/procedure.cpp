#include "procedure.h"

namespace spa {

Procedure::Procedure(std::shared_ptr<StmtLst> stmtLst) {
	stmtLst = stmtLst;
	index = count++;
}

bool Procedure::setStmtLst(std::shared_ptr<StmtLst> stmtLst) {
	if (stmtLst != nullptr) {
		stmtLst = stmtLst;
		return true;
	} else {
		return false;
	}
}

std::shared_ptr<StmtLst> Procedure::getStmtLst()
{
	return stmtLst;
}

} // namespace spa