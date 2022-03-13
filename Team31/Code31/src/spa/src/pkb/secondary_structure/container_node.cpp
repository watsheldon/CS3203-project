#include "container_node.h"

#include <cassert>

namespace spa {
void ContainerNode::SetParent(int p) noexcept {
    assert(p != 0);
    parent_ = p;
}
void ContainerNode::SetFirstChild(int c) noexcept {
    assert(c != 0);
    first_child_ = c;
}
void ContainerNode::SetNextSibling(int s) noexcept {
    assert(s != 0);
    next_sibling_ = s;
}
int ContainerNode::GetParent() const noexcept { return parent_; }
int ContainerNode::GetFirstChild() const noexcept { return first_child_; }
int ContainerNode::GetNextSibling() const noexcept { return next_sibling_; }
}  // namespace spa
