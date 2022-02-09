#include "indexed_node.h"

namespace spa {
template <typename T>
IndexedNode<T>::IndexedNode() : index(++count_) {}
template <typename T>
int IndexedNode<T>::get_index() const {
    return index;
}
}  // namespace spa
