#ifndef SRC_SPA_SRC_SOURCE_CONSTANT_H_
#define SRC_SPA_SRC_SOURCE_CONSTANT_H_

#include "indexed_ast_node.h"

#include <string>
#include <cstring>

namespace spa {

class Constant : public IndexedAstNode<Constant> {
  public:
	bool setValue(std::string value);
	std::string getValue();

  private:
	std::string value_;
};

} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_CONSTANT_H_
