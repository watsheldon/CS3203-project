#include "condition_node.h"

#include "constant_node.h"
#include "variable_node.h"

namespace spa {

void ConditionNode::AddVariable(const VariableNode *variable) noexcept {
    variables_.emplace(variable);
}
void ConditionNode::AddConstant(const ConstantNode *constant) noexcept {
    constants_.emplace(constant);
}
const std::set<const VariableNode *> &ConditionNode::GetVariables()
        const noexcept {
    return variables_;
}
const std::set<const ConstantNode *> &ConditionNode::GetConstants()
        const noexcept {
    return constants_;
}
}  // namespace spa
