#include "indexed_ast_node.h"

namespace spa {

template <typename T>
IndexedAstNode<T>::IndexedAstNode() {
	index = ++count;
}

template <typename T>
int IndexedAstNode<T>::getIndex() const {
	return index;
}

} // namespace spa
