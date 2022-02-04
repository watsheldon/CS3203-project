#include "container_node.h"
using namespace spa;

ContainerNode::ContainerNode() {
    parent = -1;
    first_child = -1;
    first_sibling = -1;
}

void ContainerNode::setParent(int p) {
    parent = p;
}

void ContainerNode::setChild(int c) {
    first_child = c;
}

void ContainerNode::setSibling(int s) {
    first_sibling = s;
}

int ContainerNode::getParent() {
    return parent;
}

int ContainerNode::getChild() {
    return first_child;
}

int ContainerNode::getSibling() {
    return first_sibling;
}
