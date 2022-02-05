#include "container_node.h"
using namespace spa;

ContainerNode::ContainerNode() {
    parent = 0;
    first_child = 0;
    first_sibling = 0;
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
