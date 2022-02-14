#include "statement_node.h"

#include <cassert>

#include "common/polish_notation.h"
#include "condition_node.h"
#include "procedure_node.h"
#include "stmt_lst_node.h"
#include "variable_node.h"
#include "visitor.h"

namespace spa {
StatementNode::~StatementNode() = default;
void AssignNode::SetModifiedVar(const VariableNode *variable) {
    assert(lhs_ == nullptr);
    lhs_ = variable;
}
void AssignNode::SetExpr(const PolishNotation *expr) {
    assert(rhs_ == nullptr);
    rhs_ = expr;
}
const VariableNode *AssignNode::GetLhs() const { return lhs_; }
const PolishNotation *AssignNode::GetRhs() const { return rhs_; }
void AssignNode::Accept(AstVisitor &visitor) const { visitor.Visit(*this); }
void CallNode::SetProcedure(const ProcedureNode *proc) {
    assert(procedure_ == nullptr);
    procedure_ = proc;
}
const ProcedureNode *CallNode::GetProcedure() const { return procedure_; }
void CallNode::Accept(AstVisitor &visitor) const { visitor.Visit(*this); }
void IfWhileNode::SetCondition(const ConditionNode *condition) {
    assert(condition_ == nullptr);
    condition_ = condition;
}
const ConditionNode *IfWhileNode::GetCondition() const { return condition_; }
IfWhileNode::~IfWhileNode() = default;
void IfNode::AddStmtLst(const StmtLstNode *node) {
    if (if_stmt_lst_.then_lst == nullptr) {
        if_stmt_lst_.then_lst = node;
        return;
    }
    assert(if_stmt_lst_.else_lst == nullptr);
    if_stmt_lst_.else_lst = node;
}
const IfNode::IfStmtLst &IfNode::GetStmtLsts() const { return if_stmt_lst_; }
void IfNode::Accept(AstVisitor &visitor) const { visitor.Visit(*this); }
void WhileNode::AddStmtLst(const StmtLstNode *node) {
    assert(stmt_lst_ == nullptr);
    stmt_lst_ = node;
}
const StmtLstNode *WhileNode::GetStmtlst() const { return stmt_lst_; }
void WhileNode::Accept(AstVisitor &visitor) const { visitor.Visit(*this); }
void ReadPrintNode::SetVariable(const VariableNode *variable) {
    assert(variable_ == nullptr);
    variable_ = variable;
}
const VariableNode *ReadPrintNode::GetVariable() const { return variable_; }
ReadPrintNode::~ReadPrintNode() = default;
void ReadNode::Accept(AstVisitor &visitor) const { visitor.Visit(*this); }
void PrintNode::Accept(AstVisitor &visitor) const { visitor.Visit(*this); }
}  // namespace spa
