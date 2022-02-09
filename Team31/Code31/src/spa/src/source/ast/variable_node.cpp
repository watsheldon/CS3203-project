#include "variable_node.h"

#include <memory>

namespace spa {
VariableNode::VariableNode(std::string name) : name_(std::move(name)) {}
std::string VariableNode::getName() const { return name_; }
}  // namespace spa
