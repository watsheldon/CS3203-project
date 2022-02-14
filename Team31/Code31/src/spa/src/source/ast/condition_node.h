#ifndef SRC_SPA_SRC_SOURCE_AST_CONDITION_H_
#define SRC_SPA_SRC_SOURCE_AST_CONDITION_H_

#include <set>

#include "constant_node.h"
#include "variable_node.h"

namespace spa {
class ConditionNode : public AbstractSyntaxTreeNode {
  public:
    void AddVariable(const VariableNode *variable);
    void AddConstant(const ConstantNode *constant);
    [[nodiscard]] const std::set<const VariableNode *> &get_variables() const;
    [[nodiscard]] const std::set<const ConstantNode *> &get_constants() const;

  private:
    std::set<const VariableNode *> variables_;
    std::set<const ConstantNode *> constants_;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_AST_CONDITION_H_
