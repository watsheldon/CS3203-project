#include "while_statement.h"

namespace spa {

bool WhileStatement::setStmtLst(std::shared_ptr<StmtLst> stmtLst) {
	stmtLst_ = stmtLst;
	return true;
}

std::shared_ptr<StmtLst> WhileStatement::getStmtLst() {
	return stmtLst_;
}

} // namespace spa
