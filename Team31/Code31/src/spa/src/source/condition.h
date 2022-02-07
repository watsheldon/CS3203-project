#ifndef SRC_SPA_SRC_SOURCE_CONDITION_H_
#define SRC_SPA_SRC_SOURCE_CONDITION_H_

#include "indexed_ast_node.h"
#include "variable.h"
#include "constant.h"

#include <memory>
#include <vector>

namespace spa {

class Condition : public IndexedAstNode<Condition> {
  public: 
	bool addVariable(std::shared_ptr<Variable> variable);
	bool addConstant(std::shared_ptr<Constant> constant);
	SharedPtrVec<Variable> getVariables();
	SharedPtrVec<Constant> getConstants();

  private:
	SharedPtrVec<Variable> variables;
	SharedPtrVec<Constant> constants;
};

} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_CONDITION_H_
