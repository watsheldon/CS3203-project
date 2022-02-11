#include "statement_node.h"

#include <cassert>
#include <memory>
#include <utility>

#include "condition_node.h"
#include "procedure_node.h"
#include "stmt_lst_node.h"
#include "variable_node.h"
#include "visitor.h"

namespace spa {
StatementNode::~StatementNode() = default;
void AssignNode::set_lhs(std::shared_ptr<VariableNode> variable) {
    lhs_ = std::move(variable);
}
void AssignNode::AddRhsNode(std::shared_ptr<AbstractSyntaxTreeNode> node) {
    rhs_.emplace_back(std::move(node));
}
std::shared_ptr<VariableNode> AssignNode::get_lhs() const { return lhs_; }
AbstractSyntaxTreeNode::SharedPtrVec<AbstractSyntaxTreeNode>
AssignNode::get_rhs() const {
    return rhs_;
}
void AssignNode::Accept(AstVisitor &visitor) const { visitor.Visit(*this); }
void CallNode::setProcedure(std::shared_ptr<ProcedureNode> proc) {
    procedure_ = std::move(proc);
}
std::shared_ptr<ProcedureNode> CallNode::get_procedure() const {
    return procedure_;
}
void CallNode::Accept(AstVisitor &visitor) const { visitor.Visit(*this); }
void IfWhileNode::set_condition(std::shared_ptr<ConditionNode> condition) {
    condition_ = std::move(condition);
}
std::shared_ptr<ConditionNode> IfWhileNode::get_condition() const {
    return condition_;
}
IfWhileNode::~IfWhileNode() = default;
void IfNode::AddStmtLst(const StmtLstNode *node) {
    if (if_stmt_lst_.then_lst == nullptr) {
        if_stmt_lst_.then_lst = node;
        return;
    }
    assert(if_stmt_lst_.else_stmt_lst == nullptr);
    if_stmt_lst_.else_stmt_lst = node;
}
const IfNode::IfStmtLst &IfNode::GetStmtLsts() const { return if_stmt_lst_; }
void IfNode::Accept(AstVisitor &visitor) const { visitor.Visit(*this); }
void WhileNode::AddStmtLst(const StmtLstNode *node) {
    assert(stmt_lst_ == nullptr);
    stmt_lst_ = node;
}
const StmtLstNode *WhileNode::GetStmtlst() const { return stmt_lst_; }
void WhileNode::Accept(AstVisitor &visitor) const { visitor.Visit(*this); }
void ReadPrintNode::set_variable(std::shared_ptr<VariableNode> variable) {
    variable_ = std::move(variable);
}
std::shared_ptr<VariableNode> ReadPrintNode::get_variable() const {
    return variable_;
}
ReadPrintNode::~ReadPrintNode() = default;
void ReadNode::Accept(AstVisitor &visitor) const { visitor.Visit(*this); }
void PrintNode::Accept(AstVisitor &visitor) const { visitor.Visit(*this); }
}  // namespace spa
