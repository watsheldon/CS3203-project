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
    StatementNode() noexcept = default;
    ~StatementNode() override = 0;
};
class AssignNode : public StatementNode {
  public:
    AssignNode() noexcept = default;
    void SetModifiedVar(const VariableNode *variable) noexcept;
    void SetExpr(const PolishNotation *expr) noexcept;
    void SetRhsIndex(int rhs_index) noexcept;
    [[nodiscard]] const VariableNode *GetLhs() const noexcept;
    [[nodiscard]] const PolishNotation *GetRhs() const noexcept;
    [[nodiscard]] int GetRhsIndex() const noexcept;
    void Accept(AstVisitor &visitor) const noexcept override;

  private:
    const VariableNode *lhs_{};
    const PolishNotation *rhs_{};
    int rhs_index_{};
};
class CallNode : public StatementNode {
  public:
    CallNode() noexcept = default;
    void SetProcedure(const ProcedureNode *proc) noexcept;
    [[nodiscard]] const ProcedureNode *GetProcedure() const noexcept;
    void Accept(AstVisitor &visitor) const noexcept override;

  private:
    const ProcedureNode *procedure_{};
};
class IfWhileNode : public StatementNode, public StmtLstParent {
  public:
    IfWhileNode() noexcept = default;
    void SetCondition(const ConditionNode *condition) noexcept;
    [[nodiscard]] const ConditionNode *GetCondition() const noexcept;
    ~IfWhileNode() override = 0;

  private:
    const ConditionNode *condition_{};
};
class IfNode : public IfWhileNode {
  public:
    struct IfStmtLst {
        const StmtLstNode *then_lst;
        const StmtLstNode *else_lst;
    };
    IfNode() noexcept = default;
    void AddStmtLst(const StmtLstNode *node) noexcept override;
    [[nodiscard]] const IfStmtLst &GetStmtLsts() const noexcept;
    void Accept(AstVisitor &visitor) const noexcept override;

  private:
    IfStmtLst if_stmt_lst_{};
};
class WhileNode : public IfWhileNode {
  public:
    WhileNode() noexcept = default;
    void AddStmtLst(const StmtLstNode *node) noexcept override;
    [[nodiscard]] const StmtLstNode *GetStmtlst() const noexcept;
    void Accept(AstVisitor &visitor) const noexcept override;

  private:
    const StmtLstNode *stmt_lst_{};
};
class ReadPrintNode : public StatementNode {
  public:
    ReadPrintNode() noexcept = default;
    void SetVariable(const VariableNode *variable) noexcept;
    [[nodiscard]] const VariableNode *GetVariable() const noexcept;
    ~ReadPrintNode() override = 0;

  private:
    const VariableNode *variable_{};
};
class ReadNode : public ReadPrintNode {
  public:
    ReadNode() noexcept = default;
    void Accept(AstVisitor &visitor) const noexcept override;
};
class PrintNode : public ReadPrintNode {
  public:
    PrintNode() noexcept = default;
    void Accept(AstVisitor &visitor) const noexcept override;
};
}  // namespace spa

#endif  // SRC_SPA_SRC_SOURCE_AST_STATEMENT_NODE_H_
