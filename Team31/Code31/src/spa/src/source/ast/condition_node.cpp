#include "condition_node.h"

#include <utility>

#include "constant_node.h"
#include "variable_node.h"

namespace spa {

void ConditionNode::AddVariable(std::shared_ptr<VariableNode> variable) {
    variables_.emplace(std::move(variable));
}
void ConditionNode::AddConstant(std::shared_ptr<ConstantNode> constant) {
    constants_.emplace(std::move(constant));
}
ConditionNode::SharedPtrSet<VariableNode> ConditionNode::get_variables() const {
    return variables_;
}
ConditionNode::SharedPtrSet<ConstantNode> ConditionNode::get_constants() const {
    return constants_;
}
}  // namespace spa
