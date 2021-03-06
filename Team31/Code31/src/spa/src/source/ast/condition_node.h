#ifndef SRC_SPA_SRC_SOURCE_AST_CONDITION_NODE_H_
#define SRC_SPA_SRC_SOURCE_AST_CONDITION_NODE_H_

#include <set>

#include "constant_node.h"
#include "variable_node.h"

namespace spa {
class ConditionNode : public AbstractSyntaxTreeNode {
  public:
    void AddVariable(const VariableNode *variable) noexcept;
    void AddConstant(const ConstantNode *constant) noexcept;
    [[nodiscard]] const std::set<const VariableNode *> &GetVariables()
            const noexcept;
    [[nodiscard]] const std::set<const ConstantNode *> &GetConstants()
            const noexcept;

  private:
    std::set<const VariableNode *> variables_;
    std::set<const ConstantNode *> constants_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_AST_CONDITION_NODE_H_
