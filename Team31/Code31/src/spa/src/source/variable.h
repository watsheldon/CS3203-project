#ifndef SRC_SPA_SRC_SOURCE_VARIABLE_H_
#define SRC_SPA_SRC_SOURCE_VARIABLE_H_

#include "indexed_ast_node.h"

#include <string>
#include <cstring>

namespace spa {

class Variable : public IndexedAstNode<Variable> {
  public:
	bool setName(std::string name);
	std::string getName();

  private:
	std::string name_;
};

} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_VARIABLE_H_
