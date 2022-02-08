#ifndef SRC_SPA_SRC_SOURCE_AST_STATEMENT_NODE_H_
#define SRC_SPA_SRC_SOURCE_AST_STATEMENT_NODE_H_

#include <memory>

#include "condition_node.h"
#include "indexed_node.h"
#include "procedure_node.h"
#include "stmt_lst_node.h"
#include "variable_node.h"

namespace spa {
class ProcedureNode;
class StmtLstNode;
class StatementNode : public IndexedNode<StatementNode> {
  public:
    ~StatementNode() override = 0;
};
class AssignNode : public StatementNode {
  public:
    void set_lhs(std::shared_ptr<VariableNode> variable);
    void AddRhsNode(std::shared_ptr<AbstractSyntaxTreeNode> node);
    [[nodiscard]] std::shared_ptr<VariableNode> get_lhs() const;
    [[nodiscard]] SharedPtrVec <AbstractSyntaxTreeNode> get_rhs() const;

  private:
    std::shared_ptr<VariableNode> lhs_;
    SharedPtrVec <AbstractSyntaxTreeNode> rhs_;
};
class CallNode : public StatementNode {
  public:
    void setProcedure(std::shared_ptr<ProcedureNode> proc);
    [[nodiscard]] std::shared_ptr<ProcedureNode> get_procedure() const;

  private:
    std::shared_ptr<ProcedureNode> procedure_;
};
class ContainerNode : public StatementNode {
  public:
    void set_condition(std::shared_ptr<ConditionNode> condition);
    [[nodiscard]] std::shared_ptr<ConditionNode> get_condition() const;
    virtual ~ContainerNode() = 0;

  protected:
    std::shared_ptr<ConditionNode> condition_;
};
class IfNode : public ContainerNode {
  public:
    void set_then(std::shared_ptr<StmtLstNode> thenLst);
    void set_else(std::shared_ptr<StmtLstNode> elseLst);
    [[nodiscard]] std::shared_ptr<StmtLstNode> get_then() const;
    [[nodiscard]] std::shared_ptr<StmtLstNode> get_else() const;

  private:
    std::shared_ptr<StmtLstNode> then_;
    std::shared_ptr<StmtLstNode> else_;
};
class WhileNode : public ContainerNode {
  public:
    void set_stmtlst(std::shared_ptr<StmtLstNode> stmtLst);
    [[nodiscard]] std::shared_ptr<StmtLstNode> get_stmtlst() const;

  private:
    std::shared_ptr<StmtLstNode> stmt_lst_;
};
class ReadPrintNode : public StatementNode {
  public:
    void set_variable(std::shared_ptr<VariableNode> variable);
    [[nodiscard]] std::shared_ptr<VariableNode> get_variable() const;
    virtual ~ReadPrintNode() = 0;

  private:
    std::shared_ptr<VariableNode> variable_;
};
class ReadNode : public ReadPrintNode {};
class PrintNode : public ReadPrintNode {};
} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_AST_STATEMENT_NODE_H_
