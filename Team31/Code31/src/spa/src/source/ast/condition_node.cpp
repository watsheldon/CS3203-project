#include "condition_node.h"

#include "constant_node.h"
#include "variable_node.h"

namespace spa {

void ConditionNode::AddVariable(const VariableNode *variable) {
    variables_.emplace(variable);
}
void ConditionNode::AddConstant(const ConstantNode *constant) {
    constants_.emplace(constant);
}
const std::set<const VariableNode *> &ConditionNode::GetVariables() const {
    return variables_;
}
const std::set<const ConstantNode *> &ConditionNode::GetConstants() const {
    return constants_;
}
}  // namespace spa
