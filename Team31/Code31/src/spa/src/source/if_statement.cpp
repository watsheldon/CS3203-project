#include "if_statement.h"

namespace spa {

bool IfStatement::setThenLst(std::shared_ptr<StmtLst> thenLst) {
	thenLst_ = thenLst;
	return true;
}

bool IfStatement::setElseLst(std::shared_ptr<StmtLst> elseLst) {
	elseLst_ = elseLst;
	return true;
}

std::shared_ptr<StmtLst> IfStatement::getThenLst() {
	return thenLst_;
}

std::shared_ptr<StmtLst> IfStatement::getElseLst() {
	return elseLst_;
}

} // namespace spa
