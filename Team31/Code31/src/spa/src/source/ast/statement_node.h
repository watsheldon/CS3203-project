#ifndef SRC_SPA_SRC_SOURCE_AST_STATEMENT_NODE_H_
#define SRC_SPA_SRC_SOURCE_AST_STATEMENT_NODE_H_

#include "common/polish_notation.h"
#include "condition_node.h"
#include "indexed_node.h"
#include "procedure_node.h"
#include "stmt_lst_node.h"
#include "stmt_lst_parent.h"
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
    void SetModifiedVar(const VariableNode *variable);
    void SetExpr(const PolishNotation *expr);
    [[nodiscard]] const VariableNode *GetLhs() const;
    [[nodiscard]] const PolishNotation *GetRhs() const;
    void Accept(AstVisitor &visitor) const override;

  private:
    const VariableNode *lhs_;
    const PolishNotation *rhs_;
};
class CallNode : public StatementNode {
  public:
    void SetProcedure(const ProcedureNode *proc);
    [[nodiscard]] const ProcedureNode *GetProcedure() const;
    void Accept(AstVisitor &visitor) const override;

  private:
    const ProcedureNode *procedure_;
};
class IfWhileNode : public StatementNode, public StmtLstParent {
  public:
    void SetCondition(const ConditionNode *condition);
    [[nodiscard]] const ConditionNode *GetCondition() const;
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
    const IfStmtLst if_stmt_lst_;
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
    void SetVariable(const VariableNode *variable);
    [[nodiscard]] const VariableNode *GetVariable() const;
    ~ReadPrintNode() override = 0;

  private:
    const VariableNode *variable_;
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
