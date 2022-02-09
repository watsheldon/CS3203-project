#include "stmt_lst_node.h"

#include <memory>
#include <utility>

#include "abstract_syntax_tree_node.h"
#include "statement_node.h"

namespace spa {
void StmtLstNode::AddStatement(std::shared_ptr<StatementNode> statement) {
    statements_.emplace_back(std::move(statement));
}
AbstractSyntaxTreeNode::SharedPtrVec<StatementNode>
StmtLstNode::get_statements() const {
    return statements_;
}
void StmtLstNode::Accept(AstVisitor &visitor) const { visitor.Visit(*this); }
}  // namespace spa
