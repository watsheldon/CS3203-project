#include "procedure_node.h"

#include <cassert>

#include "stmt_lst_node.h"
#include "visitor.h"

namespace spa {
const StmtLstNode *ProcedureNode::GetStmtlst() const noexcept {
    return stmt_lst_;
}
void ProcedureNode::Accept(AstVisitor &visitor) const noexcept {
    visitor.Visit(*this);
}
void ProcedureNode::AddStmtLst(const StmtLstNode *node) noexcept {
    assert(stmt_lst_ == nullptr);
    stmt_lst_ = node;
}
}  // namespace spa
