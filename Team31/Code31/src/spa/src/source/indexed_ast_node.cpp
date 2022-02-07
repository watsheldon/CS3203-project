#include "indexed_ast_node.h"
using namespace spa;

template <typename T>
int IndexedAstNode<T>::getIndex() const {
	return index;
}