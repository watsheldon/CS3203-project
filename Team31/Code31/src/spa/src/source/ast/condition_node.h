#ifndef SRC_SPA_SRC_SOURCE_AST_CONDITION_H_
#define SRC_SPA_SRC_SOURCE_AST_CONDITION_H_

#include <memory>
#include <vector>
#include <set>

#include "constant_node.h"
#include "variable_node.h"

namespace spa {
class ConditionNode : public AbstractSyntaxTreeNode {
  public:
    template<typename T>
    using SharedPtrSet = std::set<std::shared_ptr<T>>;
    void AddVariable(std::shared_ptr<VariableNode> variable);
    void AddConstant(std::shared_ptr<ConstantNode> constant);
    [[nodiscard]] SharedPtrSet<VariableNode> get_variables() const;
    [[nodiscard]] SharedPtrSet<ConstantNode> get_constants() const;

  private:
    SharedPtrSet<VariableNode> variables_;
    SharedPtrSet<ConstantNode> constants_;
};
} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_AST_CONDITION_H_
