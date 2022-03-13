#include "stmt_lst_node.h"

#include "statement_node.h"
#include "visitor.h"

namespace spa {
void StmtLstNode::AddStatement(const StatementNode *statement) noexcept {
    statements_.emplace_back(statement);
}
const std::vector<const StatementNode *> &StmtLstNode::GetStatements()
        const noexcept {
    return statements_;
}
void StmtLstNode::Accept(AstVisitor &visitor) const noexcept {
    visitor.Visit(*this);
}
}  // namespace spa
