#include "assign.h"

namespace spa {

void Assign::setLhs(std::shared_ptr<Variable> variable) {
	lhs = variable;
}

std::shared_ptr<Variable> Assign::getLhs() {
	return lhs;
}

bool Assign::addAstNode(std::shared_ptr<AstNode> node) {
	rhs.push_back(node);
	return true;
}

SharedPtrVec<AstNode> Assign::getRhs() {
	return rhs;
}

} // namespace spa
