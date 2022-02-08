#include "statement_node.h"

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
std::shared_ptr<VariableNode> AssignNode::get_lhs() const {
    return lhs_;
}
AbstractSyntaxTreeNode::SharedPtrVec<AbstractSyntaxTreeNode> AssignNode::get_rhs() const {
    return rhs_;
}
void AssignNode::Accept(AstVisitor &visitor) const {
    visitor.Visit(*this);
}
void CallNode::setProcedure(std::shared_ptr<ProcedureNode> proc) {
    procedure_ = std::move(proc);
}
std::shared_ptr<ProcedureNode> CallNode::get_procedure() const {
    return procedure_;
}
void CallNode::Accept(AstVisitor &visitor) const {
    visitor.Visit(*this);
}
void ContainerNode::set_condition(std::shared_ptr<ConditionNode> condition) {
    condition_ = std::move(condition);
}
std::shared_ptr<ConditionNode> ContainerNode::get_condition() const {
    return condition_;
}
ContainerNode::~ContainerNode() = default;
void IfNode::set_then(std::shared_ptr<StmtLstNode> thenLst) {
    then_ = std::move(thenLst);
}
void IfNode::set_else(std::shared_ptr<StmtLstNode> elseLst) {
    else_ = std::move(elseLst);
}
std::shared_ptr<StmtLstNode> IfNode::get_then() const {
    return then_;
}
std::shared_ptr<StmtLstNode> IfNode::get_else() const {
    return else_;
}
void IfNode::Accept(AstVisitor &visitor) const {
    visitor.Visit(*this);
}
void WhileNode::set_stmtlst(std::shared_ptr<StmtLstNode> stmtLst) {
    stmt_lst_ = std::move(stmtLst);
}
std::shared_ptr<StmtLstNode> WhileNode::get_stmtlst() const {
    return stmt_lst_;
}
void WhileNode::Accept(AstVisitor &visitor) const {
    visitor.Visit(*this);
}
void ReadPrintNode::set_variable(std::shared_ptr<VariableNode> variable) {
    variable_ = std::move(variable);
}
std::shared_ptr<VariableNode> ReadPrintNode::get_variable() const {
    return variable_;
}
ReadPrintNode::~ReadPrintNode() = default;
void ReadNode::Accept(AstVisitor &visitor) const {
    visitor.Visit(*this);
}
void PrintNode::Accept(AstVisitor &visitor) const {
    visitor.Visit(*this);
}
} // namespace spa
