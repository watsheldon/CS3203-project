#ifndef SRC_SPA_SRC_SOURCE_AST_STATEMENT_NODE_H_
#define SRC_SPA_SRC_SOURCE_AST_STATEMENT_NODE_H_

#include <memory>

#include "condition_node.h"
#include "stmt_lst_parent.h"
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
    [[nodiscard]] SharedPtrVec<AbstractSyntaxTreeNode> get_rhs() const;
    void Accept(AstVisitor &visitor) const override;

  private:
    std::shared_ptr<VariableNode> lhs_;
    SharedPtrVec<AbstractSyntaxTreeNode> rhs_;
};
class CallNode : public StatementNode {
  public:
    void setProcedure(std::shared_ptr<ProcedureNode> proc);
    [[nodiscard]] std::shared_ptr<ProcedureNode> get_procedure() const;
    void Accept(AstVisitor &visitor) const override;

  private:
    std::shared_ptr<ProcedureNode> procedure_;
};
class IfWhileNode : public StatementNode, public StmtLstParent {
  public:
    void set_condition(std::shared_ptr<ConditionNode> condition);
    [[nodiscard]] std::shared_ptr<ConditionNode> get_condition() const;
    ~IfWhileNode() override = 0;

  private:
    const ConditionNode *condition_;
};
class IfNode : public IfWhileNode {
  public:
    struct IfStmtLst {
        const StmtLstNode *then_lst;
        const StmtLstNode *else_lst;
    };
    void AddStmtLst(const StmtLstNode *node) override;
    [[nodiscard]] const IfStmtLst &GetStmtLsts() const;
    void Accept(AstVisitor &visitor) const override;

  private:
    IfStmtLst if_stmt_lst_;
};
class WhileNode : public IfWhileNode {
  public:
    void AddStmtLst(const StmtLstNode *node) override;
    [[nodiscard]] const StmtLstNode *GetStmtlst() const;
    void Accept(AstVisitor &visitor) const override;

  private:
    const StmtLstNode *stmt_lst_;
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
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_AST_STATEMENT_NODE_H_
