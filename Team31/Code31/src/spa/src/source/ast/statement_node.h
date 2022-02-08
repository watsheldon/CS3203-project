#ifndef SRC_SPA_SRC_SOURCE_AST_STATEMENT_NODE_H_
#define SRC_SPA_SRC_SOURCE_AST_STATEMENT_NODE_H_

#include <memory>

#include "condition_node.h"
#include "indexed_node.h"
#include "procedure_node.h"
#include "stmt_lst_node.h"
#include "variable_node.h"
#include "visitable.h"
#include "visitor.h"

namespace spa {
class ProcedureNode;
class StmtLstNode;
class StatementNode : public IndexedNode<StatementNode>, public Visitable {
  public:
    ~StatementNode() override = 0;
};
class AssignNode : public StatementNode {
  public:
    void set_lhs(std::shared_ptr<VariableNode> variable);
    void AddRhsNode(std::shared_ptr<AbstractSyntaxTreeNode> node);
    [[nodiscard]] std::shared_ptr<VariableNode> get_lhs() const;
    [[nodiscard]] SharedPtrVec <AbstractSyntaxTreeNode> get_rhs() const;
    void Accept(AstVisitor &visitor) const override;

  private:
    std::shared_ptr<VariableNode> lhs_;
    SharedPtrVec <AbstractSyntaxTreeNode> rhs_;
};
class CallNode : public StatementNode {
  public:
    void setProcedure(std::shared_ptr<ProcedureNode> proc);
    [[nodiscard]] std::shared_ptr<ProcedureNode> get_procedure() const;
    void Accept(AstVisitor &visitor) const override;
  private:
    std::shared_ptr<ProcedureNode> procedure_;
};
class ContainerNode : public StatementNode {
  public:
    void set_condition(std::shared_ptr<ConditionNode> condition);
    [[nodiscard]] std::shared_ptr<ConditionNode> get_condition() const;
    ~ContainerNode() override = 0;

  protected:
    std::shared_ptr<ConditionNode> condition_;
};
class IfNode : public ContainerNode {
  public:
    void set_then(std::shared_ptr<StmtLstNode> thenLst);
    void set_else(std::shared_ptr<StmtLstNode> elseLst);
    [[nodiscard]] std::shared_ptr<StmtLstNode> get_then() const;
    [[nodiscard]] std::shared_ptr<StmtLstNode> get_else() const;
    void Accept(AstVisitor &visitor) const override;

  private:
    std::shared_ptr<StmtLstNode> then_;
    std::shared_ptr<StmtLstNode> else_;
};
class WhileNode : public ContainerNode {
  public:
    void set_stmtlst(std::shared_ptr<StmtLstNode> stmtLst);
    [[nodiscard]] std::shared_ptr<StmtLstNode> get_stmtlst() const;
    void Accept(AstVisitor &visitor) const override;

  private:
    std::shared_ptr<StmtLstNode> stmt_lst_;
};
class ReadPrintNode : public StatementNode {
  public:
    void set_variable(std::shared_ptr<VariableNode> variable);
    [[nodiscard]] std::shared_ptr<VariableNode> get_variable() const;
    ~ReadPrintNode() override = 0;

  private:
    std::shared_ptr<VariableNode> variable_;
};
class ReadNode : public ReadPrintNode {
  public:
    void Accept(AstVisitor &visitor) const override;
};
class PrintNode : public ReadPrintNode {
  public:
    void Accept(AstVisitor &visitor) const override;
};
} // namespace spa

#endif // SRC_SPA_SRC_SOURCE_AST_STATEMENT_NODE_H_
