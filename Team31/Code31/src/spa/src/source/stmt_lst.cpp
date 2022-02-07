#include "stmt_lst.h"

namespace spa {

bool StmtLst::setStmtLst(std::shared_ptr<StmtLst> stmtLst) {
	stmtLst_ = stmtLst;
	return true;
}

bool StmtLst::addStatement(std::shared_ptr<Statement> statement) {
	statements.push_back(statement);
	return true;
}

SharedPtrVec<Statement> StmtLst::getStatements() {
	return statements;
}

} // namespace spa
