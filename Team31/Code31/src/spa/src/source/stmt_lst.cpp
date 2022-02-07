#include "stmt_lst.h"

namespace spa {

void StmtLst::setName(std::string name) {
	name = name;
}

std::string StmtLst::getName() {
	return name;
}

bool StmtLst::setStmtLst(std::shared_ptr<StmtLst> stmtLst) {
	if (stmtLst != nullptr) {
		stmtLst = stmtLst;
		return true;
	} else {
		return false;
	}
}

bool StmtLst::addStatement(std::shared_ptr<Statement> statement) {
	if (statement != nullptr) {
		statements.push_back(statement);
		return true;
	} else {
		return false;
	}
}

SharedPtrVec<Statement> StmtLst::getStatements() {
	return statements;
}

} // namespace spa
