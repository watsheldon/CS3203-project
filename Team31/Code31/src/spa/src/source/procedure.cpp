#include "procedure.h"

namespace spa {

Procedure::Procedure(std::shared_ptr<StmtLst> stmtLst) {
	stmtLst_ = stmtLst;
}

bool Procedure::setStmtLst(std::shared_ptr<StmtLst> stmtLst) {
	stmtLst_ = stmtLst;
	return true;
}

std::shared_ptr<StmtLst> Procedure::getStmtLst() {
	return stmtLst_;
}

} // namespace spa
