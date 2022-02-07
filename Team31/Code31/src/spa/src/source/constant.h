#ifndef SRC_SPA_SRC_SOURCE_CONSTANT_H_
#define SRC_SPA_SRC_SOURCE_CONSTANT_H_

#include "indexed_ast_node.h"

#include <string>
#include <cstring>

namespace spa {

class Constant : public IndexedAstNode<Constant> {
	public:
	void setName(std::string name);
	std::string getName();

	private:
	std::string name;
};

} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_CONSTANT_H_