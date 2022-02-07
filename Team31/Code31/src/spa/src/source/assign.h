#ifndef SRC_SPA_SRC_SOURCE_ASSIGN_H_
#define SRC_SPA_SRC_SOURCE_ASSIGN_H_

#include "statement.h"
#include "variable.h"
#include "ast_node.h"

#include <memory>

namespace spa {

class Assign : public Statement {
  public:
	void setLhs(std::shared_ptr<Variable> variable);
	std::shared_ptr<Variable> getLhs();
	bool addAstNode(std::shared_ptr<AstNode> node);
	SharedPtrVec<AstNode> getRhs();

  private:
	std::shared_ptr<Variable> lhs;
	SharedPtrVec<AstNode> rhs;
};

} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_ASSIGN_H_
