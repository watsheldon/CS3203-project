#include "container_node.h"

#include <cassert>

namespace spa {
void ContainerNode::SetParent(int p) {
    assert(p != 0);
    parent_ = p;
}
void ContainerNode::SetFirstChild(int c) {
    assert(c != 0);
    first_child_ = c;
}
void ContainerNode::SetNextSibling(int s) {
    assert(s != 0);
    next_sibling_ = s;
}
int ContainerNode::GetParent() const { return parent_; }
int ContainerNode::GetFirstChild() const { return first_child_; }
int ContainerNode::GetNextSibling() const { return next_sibling_; }
}  // namespace spa
