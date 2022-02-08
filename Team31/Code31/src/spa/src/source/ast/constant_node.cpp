#include "constant_node.h"

#include <utility>

namespace spa {
ConstantNode::ConstantNode(std::string value) : value_(std::move(value)) {}
std::string ConstantNode::getValue() const {
    return value_;
}
} // namespace spa
